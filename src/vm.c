#include <math.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#include "vm.h"
#include "debug.h"
#include "object.h"
#include "memory.h"
#include "compiler.h"
#include "native.h"
#include "error.h"

#define READ_BYTE()     (*frame->ip++)
#define READ_CONSTANT() (frame->closure->function->code.constants.values[READ_BYTE()]) 
#define READ_SHORT()    (frame->ip += 2, \
                        (uint16_t)(frame->ip[-2] << 8) | frame->ip[-1]) 
#define READ_STRING()   AS_STRING(READ_CONSTANT())

VM vm;
CallFrame *frame;

static void printErrorInCode(int offset) {
    int index = frame->ip - frame->closure->function->code.code - 1 + offset;
    int line = frame->closure->function->code.lines[index];
    int column = frame->closure->function->code.columns[index];
    int length = frame->closure->function->code.lengths[index];
    printHighlightedPartInCode(vm.source, line, column, length); 
    exit(1);
}

static void resetStack() {
    vm.top = vm.stack;
    vm.frameSize = 0;
    vm.openUpvalues = NULL;
}

void push(Value value) {
    *vm.top = value;
    vm.top++;
}

Value pop() {
    vm.top--;
    return *vm.top;
}

static Value peek(int distance) {
    return vm.top[-1 - distance];
}

static bool call(ObjClosure *closure, int argCount) {
    if (argCount != closure->function->arity) {
        reportRuntimeError("Expected %d arguments but go %d", closure->function->arity, argCount);
        printErrorInCode(0);
    }
    if (vm.frameSize == FRAMES_SIZE) {
        reportRuntimeError("Stack overflow");
        printErrorInCode(0);
    }
    CallFrame *frame = &vm.frames[vm.frameSize++];
    frame->closure = closure;
    frame->ip = closure->function->code.code;
    frame->slots = vm.top - argCount - 1;
    return true;
}

static void defineNative(const char *name, NativeFn function) {
    push(OBJ_VAL(copyString(name, (int)strlen(name))));
    ObjNative *native = createNative(function, name);
    push(OBJ_VAL(native));
    tableSet(&vm.globals, AS_STRING(vm.stack[0]), vm.stack[1]);
    pop();
    pop();
}

static void defineNatives() {
    defineNative("clock", clockNative);
    defineNative("print", printNative);
    defineNative("sqrt", sqrtNative);
    defineNative("min", minNative);
    defineNative("max", maxNative);
    defineNative("type", typeNative);
    defineNative("len", lenNative);
    defineNative("hash", hashNative);
}

static bool callValue(Value callee, int argc) {
    if (IS_OBJ(callee)) {
        switch (OBJ_TYPE(callee)) {
            case OBJ_METHOD: {
                ObjMethod *method = AS_METHOD(callee);
                vm.top[-argc - 1] = method->reciever;
                return call(method->method, argc);
            }
            case OBJ_NATIVE_METHOD: {
                ObjNativeMethod *method = AS_NATIVE_METHOD(callee);
                vm.top[-argc - 1] = method->reciever;
                NativeFn native = method->method;
                Value result = native(argc, vm.top - argc);
                vm.top -= argc + 1;
                push(result);
                return true;
            }
            case OBJ_CLASS: {
                ObjClass *class = AS_CLASS(callee);
                vm.top[-argc - 1] = OBJ_VAL(createInstance(class));
                Value initializer;
                if (tableGet(&class->methods, vm.initString, &initializer)) {
                    return call(AS_CLOSURE(initializer), argc);
                } else if (argc != 0) {
                    reportRuntimeError("Expect 0 arguments but got %d", argc);
                    return false;
                }
                return true;
            }
            case OBJ_CLOSURE:
                return call(AS_CLOSURE(callee), argc);
            case OBJ_NATIVE:
                NativeFn native = AS_NATIVE(callee)->function;
                Value result = native(argc, vm.top - argc);
                vm.top -= argc + 1;
                push(result);
                return true;
            default:
                break;
        }
    }
    reportRuntimeError("Can only call functions and classes");
    printErrorInCode(0);
    return false;
}

static void bindMethod(ObjClass *class, ObjString *name) {
    Value method;
    if (!tableGet(&class->methods, name, &method)) {
        reportRuntimeError("Undefined property '%s'", name->chars);
        printErrorInCode(0);
    }

    ObjMethod *methodObj = createMethod(peek(0), AS_CLOSURE(method));

    pop();
    push(OBJ_VAL(methodObj));
}

static ObjUpvalue *captureUpvalue(Value *local) {
    ObjUpvalue *prev = NULL;
    ObjUpvalue *cur = vm.openUpvalues;
    while (cur != NULL && cur->location > local) {
        prev = cur;
        cur = cur->next;
    }

    if (cur != NULL && cur->location == local)
        return cur;

    ObjUpvalue *createdUpvalue = createUpvalue(local);
    createdUpvalue->next = cur;

    if (prev == NULL) {
        vm.openUpvalues = createdUpvalue;
    } else {
        prev->next =  createdUpvalue;
    }
    return createdUpvalue;
}

static void closeUpvalues(Value *last) {
    while (vm.openUpvalues != NULL && vm.openUpvalues->location >= last) {
        ObjUpvalue *upvalue= vm.openUpvalues;
        upvalue->closed = *upvalue->location;
        upvalue->location = &upvalue->closed;
        vm.openUpvalues = upvalue->next;
    }
}

static void defineMethod(ObjString* name) {
    Value method = peek(0);
    ObjClass *class = AS_CLASS(peek(1));
    tableSet(&class->methods, name, method);
    pop();
}

static inline void concatenateStrings() {
    ObjString *b = AS_STRING(pop());
    ObjString *a = AS_STRING(pop());

    int length = a->length + b->length;

    int size = sizeof(ObjString) + length + 1;

    ObjString *result = (ObjString*)allocateObject(size, OBJ_STRING);

    memcpy(result->chars, a->chars, a->length);
    memcpy(result->chars + a->length, b->chars, b->length);
    result->chars[length] = '\0';

    result->length = length;
    result->isHashed = false;
    result->isInterned = false;

    push(OBJ_VAL(result));
}

static void concatenateArrays() {
    ObjArray *a = AS_ARRAY(pop());
    ObjArray *b = AS_ARRAY(pop());

    size_t aSize = a->values.size;
    size_t bSize = b->values.size;

    size_t size = aSize + bSize; 

    ObjArray *result = allocateArray(size);

    for (int i = 0; i < aSize; i++) {
        result->values.values[i] = a->values.values[i];
    }
    for (int i = 0; i < bSize; i++) {
        result->values.values[aSize + i] = b->values.values[i];
    }

    push(OBJ_VAL(result));
}

static inline double add(double a, double b) {
    return a + b; 
}

static inline double subtract(double a, double b) {
    return a - b;
}

static inline double multiply(double a, double b) {
    return a * b; 
}

static inline double divide(double a, double b) {
    if (b == 0) {
        reportRuntimeError("Division by zero");
        printErrorInCode(0);
    }
    return a / b;
}

static inline double modulo(double a, double b) {
    if (b == 0) {
        reportRuntimeError("Division by zero");
        printErrorInCode(0);
    }
    return fmod(a, b);
}

static void arithmetic(double (*function)(double, double)) {
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
        reportRuntimeError("Arithmetic operations are not supported for %s and %s", decodeValueType(peek(0)), decodeValueType(peek(1)));
        printErrorInCode(0);
    }

    double b = AS_NUMBER(pop());
    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(function(a, b)));
}

static void plus() {
    if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
        concatenateStrings();
    } else if (IS_ARRAY(peek(0)) && IS_ARRAY(peek(1))) {
        concatenateArrays();
    } else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
        arithmetic(add);
    } else {
        reportRuntimeError("'+' operator is not supported for %s and %s", decodeValueType(peek(0)), decodeValueType(peek(1)));
    }
}

static void repeatString() {
    ObjString* string;
    double number;

    if (IS_STRING(peek(0))) {
        string = AS_STRING(pop());
        number = AS_NUMBER(pop());
    } else {
        number = AS_NUMBER(pop());
        string = AS_STRING(pop());
    }

    if (number != (int)number) {
        reportRuntimeError("Can't multiply string by non whole number");
        printErrorInCode(0);
    }

    size_t initLength = string->length;
    size_t newLength = initLength * number;

    ObjString *result = (ObjString*)allocateObject(sizeof(ObjString) + newLength, OBJ_STRING);
    result->isHashed = false;
    result->isInterned = false;
    result->length = newLength;

    for (int i = 0; i < number; i++) {
        memcpy(result->chars + i * initLength, string->chars, initLength);
    }

    push(OBJ_VAL(result));
}

static void repeatArray() {
    ObjArray *array;
    double number;

    if (IS_ARRAY(peek(0))) {
        array = AS_ARRAY(pop());
        number = AS_NUMBER(pop());
    } else {
        number = AS_NUMBER(pop());
        array = AS_ARRAY(pop());
    }

    if (number != (int)number) {
        reportRuntimeError("Can't multiply array by non whole number");
        printErrorInCode(0);
    }

    size_t initSize = array->values.size;
    size_t newSize = initSize * number;

    ObjArray *result = allocateArray(newSize);

    for (int i = 0; i < newSize; i++) {
        result->values.values[i] = array->values.values[i % initSize];
    }

    push(OBJ_VAL(result));
}

static void star() {
    if (IS_ARRAY(peek(0)) && IS_NUMBER(peek(1)) ||
        IS_NUMBER(peek(0)) && IS_ARRAY(peek(1))) {
        repeatArray(); 
    } else if (IS_STRING(peek(0)) && IS_NUMBER(peek(1)) ||
             IS_NUMBER(peek(0)) && IS_STRING(peek(1))) {
        repeatString();
    } else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
        arithmetic(multiply);
    } else {
        reportRuntimeError("'*' operator is not supported for %s and %s", decodeValueType(peek(0)), decodeValueType(peek(1)));
    }
}

static inline void increment() {
    if (!IS_NUMBER(peek(0))) {
        reportRuntimeError("Increment is not supported for %s", decodeValueType(peek(0)));
        printErrorInCode(0);
    }

    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(a + 1));
}

static inline void decrement() {
    if (!IS_NUMBER(peek(0))) {
        reportRuntimeError("Decrement is not supported for %s", decodeValueType(peek(0)));
        printErrorInCode(0);
    }

    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(a - 1));
}

static void equality(bool inverse) {
    Value b = pop();
    Value a = pop();

    push(BOOL_VAL(compareValues(a, b) ^ inverse));
}

static inline bool less(double a, double b) {
    return a < b;
}

static inline bool lessEqual(double a, double b) {
    return a <= b; 
}

static inline bool greater(double a, double b) {
    return a > b;
}

static inline bool greaterEqual(double a, double b) {
    return a >= b;
}

static void inequality(bool (*function)(double, double)) {
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
        reportRuntimeError("Comparison between %s and %s is not supported", decodeValueType(peek(0)), decodeValueType(peek(1)));
        printErrorInCode(0);
    }

    double b = AS_NUMBER(pop());
    double a = AS_NUMBER(pop());
    
    push(BOOL_VAL(function(a, b)));
}

static bool isFalsey(Value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value)) || (IS_NUMBER(value) && AS_NUMBER(value) == 0);
}

static void buildFormattedString() {
    const size_t bufferSize = 512;
    size_t spaceLeft = bufferSize;
    int size = sizeof(ObjString) + bufferSize;

    ObjString *string = (ObjString*)allocateObject(size, OBJ_STRING);
    string->isInterned = false;
    string->isHashed = false;

    char *buffer = string->chars;
    int partCount = READ_BYTE();
    int stringSize = 0;

    for (int i = 0; i < partCount; i++) {
        Value part = peek(partCount - i - 1);
        int bytesWritten = writeValue(part, buffer, spaceLeft);
        buffer += bytesWritten;
        stringSize += bytesWritten;
        spaceLeft -= bytesWritten;
        if (spaceLeft <= 0) {
            printf("Formatted string is too big\n");
            exit(1);
        }
    }

    string->length = stringSize;

    int newSize = size - (bufferSize - stringSize);

    reallocate(string, size, newSize);

    for (int i = 0; i < partCount; i++)
        pop();

    push(OBJ_VAL(string));
}

static void buildArray() {
    size_t size = READ_BYTE();
    ObjArray *array = allocateArray(size);

    for (int i = 0; i < size; i++) {
        Value value = peek(size - i - 1);
        array->values.values[i] = value;
    }
    
    for (int i = 0; i < size; i++)
        pop();
    
    push(OBJ_VAL(array));
}

static void getAt(bool popValues) {
    Value key;
    Value object; 

    if (popValues) {
        key = pop();
        object = pop();
    } else {
        key = peek(0);
        object = peek(1);
    }

    if (!IS_OBJ(object)) {
        reportRuntimeError("%s is not subscripable", decodeValueType(object));
        printErrorInCode(0);
    }

    switch (AS_OBJ(object)->type) {
        case OBJ_ARRAY: {
            if (!IS_NUMBER(key)) {
                reportRuntimeError("Index must be a number");
                printErrorInCode(0);
            }
            float fi = AS_NUMBER(key);
            if (fi != (int)fi) {
                reportRuntimeError("Index must be a whole number");
                printErrorInCode(0);
            }
            
            int i = (int)fi;

            int size = AS_ARRAY(object)->values.size;
            if (i >= size || i < -size) {
                reportRuntimeError("Index is out of range");
                printErrorInCode(0);
            }
            if (i < 0)
                i += size;

            push(AS_ARRAY(object)->values.values[i]);
            break;
        }
        case OBJ_STRING: {
            if (!IS_NUMBER(key)) {
                reportRuntimeError("Index must be a number");
                printErrorInCode(0);
            }
            float fi = AS_NUMBER(key);
            if (fi != (int)fi) {
                reportRuntimeError("Index must be a whole number");
                printErrorInCode(0);
            }
            int i = (int)fi;

            int length = AS_STRING(object)->length; 
            if (i >= length || i < -length) {
                reportRuntimeError("Index is out of range");
                printErrorInCode(0);
            }
            if (i < 0)
                i += length;
            
            char chr = AS_STRING(object)->chars[i];
            ObjString *string = allocateString(&chr, 1);
            push(OBJ_VAL(string));
            break;
        }
        default:
            reportRuntimeError("%s is not subscripable", decodeObjType(object));
            printErrorInCode(0);
    }
}

static void setAt() {
    Value value = pop();
    Value key = pop();
    Value object = peek(0);

    if (!IS_OBJ(object)) {
        reportRuntimeError("%s is not subscripable", decodeValueType(object));
        printErrorInCode(0);
    }

    switch (AS_OBJ(object)->type) {
        case OBJ_ARRAY: {
            if (!IS_NUMBER(key)) {
                reportRuntimeError("Index must be a number");
                printErrorInCode(0);
            }
            float fi = AS_NUMBER(key);
            if (fi != (int)fi) {
                reportRuntimeError("Index must be a whole number");
                printErrorInCode(0);
            }
            int i = (int)fi;

            int size = AS_ARRAY(object)->values.size;
            if (i >= size || i < -size) {
                reportRuntimeError("Index is out of range");
                printErrorInCode(0);
            }
            if (i < 0)
                i += size;

            AS_ARRAY(object)->values.values[i] = value;
            break;
        }
        case OBJ_STRING:
            reportRuntimeError("Strings are immutable");
            printErrorInCode(0);
        default:
            reportRuntimeError("%s is not subscripable", decodeObjType(value));
            printErrorInCode(0);
    }

}

static void getProperty() {
    ObjString *name = READ_STRING();
    switch (OBJ_TYPE(peek(0))) {
        case OBJ_ARRAY: {
            ObjNativeMethod *method = NULL; 

            if (strcmp("push", name->chars) == 0)
                method = createNativeMethod(peek(0), arrayPushNative, "push");
            else if (strcmp("pop", name->chars) == 0)
                method = createNativeMethod(peek(0), arrayPopNative, "pop");

            if (method != NULL)
                push(OBJ_VAL(method));
            else
                reportRuntimeError("Array does not have field '%s'", name->chars);
            break;
        }
        case OBJ_INSTANCE: {
            ObjInstance *instance = AS_INSTANCE(peek(0));
            Value value;
            if (tableGet(&instance->fields, name, &value)) {
                pop();
                push(value);
                return;
            }

            bindMethod(instance->class, name);
            break;        
        }
        default: {
            reportRuntimeError("Object of %s does not have fields", decodeValueType(peek(0)));
        }
    }
}

static void setProperty() {
    if (!IS_INSTANCE(peek(1))) {
        reportRuntimeError("Only instances have fields");
        printErrorInCode(0);
    }
    ObjInstance *instance = AS_INSTANCE(peek(1));
    tableSet(&instance->fields, READ_STRING(), peek(0));
}

static InterpretResult run() {
    frame = &vm.frames[vm.frameSize - 1];
    while (true) {

        #ifdef DEBUG_TRACE_EXECUTION
            printf("stack: ");
            for (Value *slot = vm.stack; slot < vm.top; slot++) {
                printf("[ ");
                printValue(*slot);
                printf(" ]");
            }
            printf("\n");
            printInstruction(&frame->closure->function->code, (int)(frame->ip - frame->closure->function->code.code));
        #endif

        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT:
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            case OP_NIL:
                push(NIL_VAL);
                break;
            case OP_POP:
                pop();
                break;
            case OP_GET_GLOBAL: {
                ObjString *name = READ_STRING();
                Value value;
                if (!tableGet(&vm.globals, name, &value)) {
                    reportRuntimeError("Undefined variable '%s'", name->chars);
                    printErrorInCode(0);
                }
                push(value);
                break;
            }
            case OP_DEFINE_GLOBAL: {
                ObjString *name = READ_STRING();
                tableSet(&vm.globals, name, peek(0));
                break;
            }
            case OP_SET_GLOBAL: {
                ObjString *name = READ_STRING();
                if (tableSet(&vm.globals, name, peek(0))) {
                    tableDelete(&vm.globals, name);
                    reportRuntimeError("Undefined variable '%s'", name->chars);
                    printErrorInCode(0);
                }
                break;
            }
            case OP_GET_LOCAL: {
                uint8_t slot = READ_BYTE();
                push(frame->slots[slot]);
                break;
            }
            case OP_SET_LOCAL: {
                uint8_t slot = READ_BYTE();
                frame->slots[slot] = peek(0);
                break;
            }
            case OP_GET_UPVALUE: {
                uint8_t slot = READ_BYTE();
                push(*frame->closure->upvalues[slot]->location);
                break;
            }
            case OP_SET_UPVALUE: {
                uint8_t slot = READ_BYTE();
                *frame->closure->upvalues[slot]->location = peek(0);
                break;
            }
            case OP_GET_AT:
                getAt(true);
                break;
            case OP_GET_AT_NO_POP:
                getAt(false);
                break;
            case OP_SET_AT:
                setAt();
                break;
            case OP_FALSE:
                push(BOOL_VAL(false));
                break;
            case OP_TRUE:
                push(BOOL_VAL(true));
                break;
            case OP_EQUAL:
                equality(false);
                break;
            case OP_NOT_EQUAL:
                equality(true);
                break;
            case OP_GREATER:
                inequality(greater);
                break;
            case OP_GREATER_EQUAL:
                inequality(greaterEqual);
                break;
            case OP_LESS:
                inequality(less);
                break;
            case OP_LESS_EQUAL:
                inequality(lessEqual);
                break;
            case OP_INCREMENT:
                increment();
                break; 
            case OP_DECREMENT:
                decrement();
                break;
            case OP_ADD:
                plus();
                break;
            case OP_SUBTRUCT:
                arithmetic(subtract);
                break;
            case OP_MULTIPLY:
                star();
                break;
            case OP_DIVIDE:
                arithmetic(divide);
                break;
            case OP_MOD:
                arithmetic(modulo);
                break;
            case OP_POWER:
                arithmetic(pow);
                break;
            case OP_NOT:
                push(BOOL_VAL(isFalsey(pop())));
                break;
            case OP_NEGATE:
                if (!IS_NUMBER(peek(0))) {
                    reportRuntimeError("Negation is not supported for %s", decodeValueType(peek(0)));
                    printErrorInCode(0);
                }
                push(NUMBER_VAL(-AS_NUMBER(pop())));
                break;
            case OP_BUILD_FSTRING:
                buildFormattedString();
                break;
            case OP_BUILD_ARRAY:
                buildArray();
                break;
            case OP_JUMP: {
                uint16_t offset = READ_SHORT();
                frame->ip += offset;
                break;
            }
            case OP_JUMP_TRUE: {
                uint16_t offset = READ_SHORT();
                if (!isFalsey(peek(0))) 
                    frame->ip += offset;
                break;
            }
            case OP_JUMP_TRUE_POP: {
                uint16_t offset = READ_SHORT();
                if (isFalsey(pop())) 
                    frame->ip += offset;
                break;
            }
            case OP_JUMP_FALSE: {
                uint16_t offset = READ_SHORT();
                if (isFalsey(peek(0))) 
                    frame->ip += offset;
                break;
            }
            case OP_JUMP_FALSE_POP: {
                uint16_t offset = READ_SHORT();
                if (isFalsey(pop())) 
                    frame->ip += offset;
                break;
            }
            case OP_LOOP: {
                uint16_t offset = READ_SHORT();
                frame->ip -= offset;
                break;
            }
            case OP_LOOP_TRUE_POP: {
                uint16_t offset = READ_SHORT();
                if (!isFalsey(pop()))
                    frame->ip -= offset;
                break;
            }
            case OP_CALL: {
                int argCount = READ_BYTE();
                if (!callValue(peek(argCount), argCount))
                    return INTERPRET_RUNTIME_ERROR;
                frame = &vm.frames[vm.frameSize - 1];
                break;
            }
            case OP_CLOSURE: {
                ObjFunction *function = AS_FUNCTION(READ_CONSTANT());
                ObjClosure *closure = createClosure(function);
                push(OBJ_VAL(closure));
                for (int i = 0; i < closure->upvalueCount; i++) {
                    uint8_t isLocal = READ_BYTE();
                    uint8_t index = READ_BYTE();
                    if (isLocal) {
                        closure->upvalues[i] = captureUpvalue(frame->slots + index);
                    } else {
                        closure->upvalues[i] = frame->closure->upvalues[index];
                    }
                }
                break;
            }
            case OP_CLOSE_UPVALUE:
                closeUpvalues(vm.top - 1);
                pop();
                break;
            case OP_CLASS:
                push(OBJ_VAL(createClass(READ_STRING())));
                break;
            case OP_METHOD:
                defineMethod(READ_STRING());
                break;
            case OP_GET_PROPERTY:
                getProperty();
                break;
            case OP_SET_PROPERTY:
                setProperty();
                break;
            case OP_RETURN: {
                Value result = pop();
                closeUpvalues(frame->slots);
                vm.frameSize--;
                if (vm.frameSize == 0) {
                    pop();
                    return INTERPRET_OK;
                }

                vm.top = frame->slots;
                push(result);
                frame = &vm.frames[vm.frameSize - 1];
                break;
            }
        }
    }
}

void initVM() {
    resetStack();
    vm.objects = NULL;
    vm.bytesAllocated = 0;
    vm.nextGC = 1024 * 1024;
    initTable(&vm.globals);
    initTable(&vm.strings);
    vm.initString = NULL;
    vm.initString = copyString("init", 4);
    defineNatives();
}

void freeVM() {
    freeObjects();
    initTable(&vm.globals);
    freeTable(&vm.strings);
    vm.initString = NULL;
    return;
}

InterpretResult interpret(const char *source) {
    ObjFunction *function = compile(source);
    if (function == NULL)
        return INTERPRET_COMPILE_ERROR;

    push(OBJ_VAL(function));
    ObjClosure *closure = createClosure(function);
    pop();
    push(OBJ_VAL(closure));
    call(closure, 0);

    #ifdef DEBUG_DO_NOT_EXECUTE
        return INTERPRET_OK;
    #endif

    vm.source = source;
    return run();
}