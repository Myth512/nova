#include <math.h>
#include <string.h>
#include <time.h>

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

static void reportError(const char *message) {
    fprintf(stderr, "\033[31mRuntime Error\033[0m: %s\n", message);
    int index = frame->ip - frame->closure->function->code.code - 1;
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
        printf("Expected %d arguments but got %d\n", closure->function->arity, argCount);
        return false;
    }
    if (vm.frameSize == FRAMES_SIZE) {
        printf("Stack overflow");
        return false;
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
    // defineNative("clock", clockNative);
    defineNative("print", printNative);
    // defineNative("sqrt", sqrtNative);
    // defineNative("min", minNative);
    // defineNative("max", maxNative);
    defineNative("type", typeNative);
}

static bool callValue(Value callee, int argc) {
    if (IS_OBJ(callee)) {
        switch (OBJ_TYPE(callee)) {
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
    reportError("Can only call functions and classes");
    return false;
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

static inline void concatenate() {
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
        reportError("Division by zero");
    }
    return a / b;
}

static inline double modulo(double a, double b) {
    if (b == 0) {
        reportError("Division by zero");
    }
    return fmod(a, b);
}

static void arithmetic(double (*function)(double, double)) {
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
        reportError("Operands must be numbers");
    }

    double b = AS_NUMBER(pop());
    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(function(a, b)));
}

static inline void plus() {
    if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
        concatenate();
        return;
    }

    arithmetic(add);
}

static inline void increment() {
    if (!IS_NUMBER(peek(0))) {
        reportError("Operand must be number");
    }

    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(a + 1));
}

static inline void decrement() {
    if (!IS_NUMBER(peek(0))) {
        reportError("Operand must be number");
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
        reportError("Operands must be numbers");
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
                    reportError("Undefined variable");
                }
                push(value);
                break;
            }
            case OP_DEFINE_GLOBAL: {
                ObjString *name = READ_STRING();
                tableSet(&vm.globals, name, peek(0));
                pop();
                break;
            }
            case OP_SET_GLOBAL: {
                ObjString *name = READ_STRING();
                if (tableSet(&vm.globals, name, pop())) {
                    tableDelete(&vm.globals, name);
                    reportError("Undefined variable");
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
                frame->slots[slot] = pop();
                break;
            }
            case OP_GET_UPVALUE: {
                uint8_t slot = READ_BYTE();
                push(*frame->closure->upvalues[slot]->location);
                break;
            }
            case OP_SET_UPVALUE: {
                uint8_t slot = READ_BYTE();
                *frame->closure->upvalues[slot]->location = pop();
                break;
            }
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
                arithmetic(multiply);
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
                    reportError("Operand must be number");
                }
                push(NUMBER_VAL(-AS_NUMBER(pop())));
                break;
            case OP_BUILD_FSTRING:
                buildFormattedString();
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
        #ifdef DEBUG_TRACE_EXECUTION
            printf("instruction end\n");
        #endif
    }
}

void initVM() {
    resetStack();
    vm.objects = NULL;
    initTable(&vm.globals);
    initTable(&vm.strings);

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