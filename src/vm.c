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
#include "arrayMethods.h"

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

static void printStack() {
    printf("stack: ");
    for (Value *slot = vm.stack; slot < vm.top; slot++) {
        printf("[ ");
        printValue(*slot);
        printf(" ]");
    }
    printf("\n");
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

static bool call(ObjClosure *closure, int argc) {
    if (argc != closure->function->arity) {
        reportRuntimeError("Expected %d arguments but go %d", closure->function->arity, argc);
        printErrorInCode(0);
    }
    if (vm.frameSize == FRAMES_SIZE) {
        reportRuntimeError("Stack overflow");
        printErrorInCode(0);
    }
    CallFrame *frame = &vm.frames[vm.frameSize++];
    frame->closure = closure;
    frame->ip = closure->function->code.code;
    frame->slots = vm.top - argc - 1;
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

static bool callValueInternal(Value callee, int argc) {
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
                Value result = native(argc, vm.top - argc - 1);
                vm.top -= argc + 1;
                push(result);
                return true;
            }
            case OBJ_CLASS: {
                ObjClass *class = AS_CLASS(callee);
                vm.top[-argc - 1] = OBJ_VAL(createInstance(class));
                Value initializer;
                if (tableGet(&class->methods, vm.magicStrings.init, &initializer)) {
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

static bool callValue(Value callee, int argc) {
    bool res = callValueInternal(callee, argc);
    frame = &vm.frames[vm.frameSize - 1];
    return res;
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

static inline double increment(double a) {
    return a + 1;
}

static inline double decrement(double a) {
    return a - 1;
}

static inline double negation(double a) {
    return -a;
}

static void swapValues(Value *a, Value *b) {
    Value *tmp = a;
    *a = *b;
    *b = *tmp;
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

static bool getProperty(Value obj, ObjString *name, Value *value) {
    if (!IS_OBJ(obj))
        return false;

    switch (OBJ_TYPE(obj)) {
        case OBJ_ARRAY: {
            const struct Keyword *result = in_word_set(name->chars, strlen(name->chars));
            if (result) {
                ObjNativeMethod *method = createNativeMethod(obj, result->method, result->name);
                *value = OBJ_VAL(method);
                return true;
            } 
            return false;
        }
        case OBJ_INSTANCE: {
            ObjInstance *instance = AS_INSTANCE(obj);
            if (tableGet(&instance->fields, name, value))
                return true;

            if (tableGet(&instance->class->methods, name, value)) {
                ObjMethod *method = createMethod(obj, AS_CLOSURE(*value));
                *value = OBJ_VAL(method);
                return true;
            }

            return false;
        }
        default: {
            return false;
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

static bool callBinaryMethod(ObjString *universal, ObjString *left, ObjString *right) {
    Value method;

    if (left && getProperty(peek(1), left, &method)) {
        callValue(method, 1);
        return true;
    }

    if (universal && getProperty(peek(1), universal, &method)) {
        callValue(method, 1);
        return true;
    }

    if (right && getProperty(peek(0), right, &method)) {
        swapValues(vm.top - 1, vm.top - 2);
        callValue(method, 1);
        return true;
    }

    if (universal && getProperty(peek(0), universal, &method)) {
        swapValues(vm.top - 1, vm.top - 2);
        callValue(method, 1);
        return true;
    }

    return false;
}

static bool callUnaryMethod(ObjString *universal) {
    Value method;

    if (getProperty(peek(0), universal, &method)) {
        callValue(method, 0);
        return true;
    }

    return false;
}

static void arithmetic(char *name, double (*function)(double, double), ObjString *universal, ObjString *left, ObjString *right) {
    if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
        double b = AS_NUMBER(pop());
        double a = AS_NUMBER(pop());
        push(NUMBER_VAL(function(a, b)));
        return;
    }

    if (callBinaryMethod(universal, left, right))
        return;

    reportRuntimeError("Unsupported operator '%s' for %s and %s", name, decodeValueType(peek(1)), decodeValueType(peek(0)));
    printErrorInCode(0);
}

static inline void unary(char *name, double (*function)(double), ObjString *method) {
    if (IS_NUMBER(peek(0))) {
        double a = AS_NUMBER(pop());
        push(NUMBER_VAL(function(a)));
        return;
    }

    if (callUnaryMethod(method))
        return;

    reportRuntimeError("Unsupported operator '%s' for %s", name, decodeValueType(peek(0)));
    printErrorInCode(0);
}

static void inequality(char *name, bool (*function)(double, double), ObjString *method) {
    if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
        double b = AS_NUMBER(pop());
        double a = AS_NUMBER(pop());
        push(BOOL_VAL(function(a, b)));
        return;
    }

    if (callBinaryMethod(method, NULL, NULL))
        return;

    reportRuntimeError("Unsupported operator '%s' for %s and %s", name, decodeValueType(peek(1)), decodeValueType(peek(0)));
    printErrorInCode(0);
}

static void equality(char *name, bool inverse, ObjString *method) {
    if (callBinaryMethod(method, NULL, NULL))
        return;
    
    Value b = pop();
    Value a = pop();

    push(BOOL_VAL(compareValues(a, b) ^ inverse));
}

void printInstanse(Value value) {
    if (!callUnaryMethod(vm.magicStrings.str)) {
        printf("instance of %s", AS_INSTANCE(value)->class->name->chars);
    }
}

static InterpretResult run() {
    frame = &vm.frames[vm.frameSize - 1];
    while (true) {

        #ifdef DEBUG_TRACE_EXECUTION
            printStack();
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
                equality("!=", false, vm.magicStrings.eq);
                break;
            case OP_NOT_EQUAL:
                equality("!=", true, vm.magicStrings.ne);
                break;
            case OP_GREATER:
                inequality(">", greater, vm.magicStrings.gt);
                break;
            case OP_GREATER_EQUAL:
                inequality(">=", greaterEqual, vm.magicStrings.ge);
                break;
            case OP_LESS:
                inequality("<", less, vm.magicStrings.lt);
                break;
            case OP_LESS_EQUAL:
                inequality("<=", lessEqual, vm.magicStrings.le);
                break;
            case OP_INCREMENT:
                unary("++", increment, vm.magicStrings.inc);
                break; 
            case OP_DECREMENT:
                unary("--", decrement, vm.magicStrings.dec);
                break;
            case OP_ADD:
                arithmetic("+", add, vm.magicStrings.add, vm.magicStrings.ladd, vm.magicStrings.radd);
                break;
            case OP_SUBTRUCT:
                arithmetic("-", subtract, vm.magicStrings.sub, vm.magicStrings.lsub, vm.magicStrings.rsub);
                break;
            case OP_MULTIPLY:
                arithmetic("*", multiply, vm.magicStrings.mul, vm.magicStrings.lmul, vm.magicStrings.rmul);
                break;
            case OP_DIVIDE:
                arithmetic("/", divide, vm.magicStrings.div, vm.magicStrings.ldiv, vm.magicStrings.rdiv);
                break;
            case OP_MOD:
                arithmetic("%%", modulo, vm.magicStrings.mod, vm.magicStrings.lmod, vm.magicStrings.rmod);
                break;
            case OP_POWER:
                arithmetic("^", pow, vm.magicStrings.pow, vm.magicStrings.lpow, vm.magicStrings.rpow);
                break;
            case OP_NOT:
                push(BOOL_VAL(isFalsey(pop())));
                break;
            case OP_NEGATE:
                unary("-", negation, vm.magicStrings.neg);
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
                int argc = READ_BYTE();
                if (!callValue(peek(argc), argc))
                    return INTERPRET_RUNTIME_ERROR;
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
                Value obj = peek(0);
                ObjString *name = READ_STRING();
                Value value;
                if (getProperty(obj, name, &value)) {
                    pop();
                    push(value);
                } else {
                    reportRuntimeError("Object of %s does not have property %s", decodeValueType(obj), name->chars);
                    printErrorInCode(0);
                }
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

void initMagicStrings() {
    vm.magicStrings.init = copyString("_init_", 6);
    vm.magicStrings.add = copyString("_add_", 5);
    vm.magicStrings.ladd = copyString("_ladd_", 6);
    vm.magicStrings.radd = copyString("_radd_", 6);
    vm.magicStrings.inc = copyString("_inc_", 5);
    vm.magicStrings.dec = copyString("_dec_", 5);
    vm.magicStrings.sub = copyString("_sub_", 5);
    vm.magicStrings.lsub = copyString("_lsub_", 6);
    vm.magicStrings.rsub = copyString("_rsub_", 6);
    vm.magicStrings.neg = copyString("_neg_", 5);
    vm.magicStrings.mul = copyString("_mul_", 5);
    vm.magicStrings.lmul = copyString("_lmul_", 6);
    vm.magicStrings.rmul = copyString("_rmul_", 6);
    vm.magicStrings.div = copyString("_div_", 5);
    vm.magicStrings.ldiv = copyString("_ldiv_", 6);
    vm.magicStrings.rdiv = copyString("_rdiv_", 6);
    vm.magicStrings.mod = copyString("_mod_", 5);
    vm.magicStrings.lmod = copyString("_lmod_", 6);
    vm.magicStrings.rmod = copyString("_rmod_", 6);
    vm.magicStrings.pow = copyString("_pow_", 5);
    vm.magicStrings.lpow = copyString("_lpow_", 6);
    vm.magicStrings.rpow = copyString("_rpow_", 6);
    vm.magicStrings.eq = copyString("_eq_", 4);
    vm.magicStrings.ne = copyString("_ne_", 4);
    vm.magicStrings.lt = copyString("_lt_", 4);
    vm.magicStrings.le = copyString("_le_", 4);
    vm.magicStrings.gt = copyString("_gt_", 4);
    vm.magicStrings.ge = copyString("_ge_", 4);
    vm.magicStrings.call = copyString("_call_", 6);
    vm.magicStrings.getat = copyString("_getat_", 7);
    vm.magicStrings.setat = copyString("_setat_", 7);
    vm.magicStrings.len = copyString("_len_", 5);
    vm.magicStrings.str = copyString("_str_", 5);
}

void initVM() {
    resetStack();
    vm.objects = NULL;
    vm.bytesAllocated = 0;
    vm.nextGC = 1024 * 1024;
    initTable(&vm.globals);
    initTable(&vm.strings);
    initMagicStrings();
    defineNatives();
}

void freeVM() {
    freeObjects();
    initTable(&vm.globals);
    freeTable(&vm.strings);
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