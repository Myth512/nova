#include "vm.h"

VM vm;

static void resetStack() {
    vm.top = vm.stack;
}

static void push(Value value) {
    *vm.top = value;
    vm.top++;
}

static Value pop() {
    vm.top--;
    return *vm.top;
}

static Value peek(int distance) {
    return vm.top[-1 - distance];
}

static inline void add() {
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
        printf("Operands must be numbers\n");
        return;
    }

    double b = AS_NUMBER(pop());
    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(a + b));
}

static inline void subtract() {
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
        printf("Operands must be numbers\n");
        return;
    }

    double b = AS_NUMBER(pop());
    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(a - b));
}


static inline void multiply() {
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
        printf("Operands must be numbers\n");
        return;
    }

    double b = AS_NUMBER(pop());
    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(a * b));
}

static inline void divide() {
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
        printf("Operands must be numbers\n");
        return;
    }

    double b = AS_NUMBER(pop());
    if (b == 0) {
        printf("Division by zero");
        return;
    }
    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(a / b));
}

static inline void mod() {
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
        printf("Operands must be numbers\n");
        return;
    }

    double b = AS_NUMBER(pop());
    if (b == 0) {
        printf("Division by zero");
        return;
    }
    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(fmod(a, b)));
}

static bool valuesEqual(Value a, Value b) {
    if (a.type != b.type)
        return false;

    switch (a.type) {
        case VAL_BOOL:
            return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL:
            return true;
        case VAL_NUMBER:
            return AS_NUMBER(a) == AS_NUMBER(b);
        default:
            return false;
    }
}

static inline void equal() {
    Value b = pop();
    Value a = pop();
    push(BOOL_VAL(valuesEqual(a, b)));
}

static inline void notEqual() {
    Value b = pop();
    Value a = pop();
    push(BOOL_VAL(!valuesEqual(a, b)));
}

static inline void greater() {
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
        printf("Operands must be numbers\n");
        return;
    }

    double b = AS_NUMBER(pop());
    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(a > b));
}

static inline void greaterEqual() {
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
        printf("Operands must be numbers\n");
        return;
    }

    double b = AS_NUMBER(pop());
    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(a >= b));
}

static inline void less() {
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
        printf("Operands must be numbers\n");
        return;
    }

    double b = AS_NUMBER(pop());
    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(a > b));
}

static inline void lessEqual() {
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
        printf("Operands must be numbers\n");
        return;
    }

    double b = AS_NUMBER(pop());
    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(a <= b));
}

static bool isFalsey(Value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value)) || (IS_NUMBER(value) && AS_NUMBER(value) == 0);
}

static InterpretResult run() {
    #define READ_BYTE() (*vm.ip++)
    #define READ_CONSTANT() (vm.code->constants.values[READ_BYTE()])

    while (true) {

        #ifdef DEBUG_TRACE_EXECUTION
            printf("stack: ");
            for (Value *slot = vm.stack; slot < vm.top; slot++) {
                printf("[ ");
                printValue(*slot);
                printf(" ]");
            }
            printf("\n");
            printInstruction(vm.code, (int)(vm.ip - vm.code->code));
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
            case OP_FALSE:
                push(BOOL_VAL(false));
                break;
            case OP_TRUE:
                push(BOOL_VAL(true));
                break;
            case OP_EQUAL:
                equal();
                break;
            case OP_NOT_EQUAL:
                notEqual();
                break;
            case OP_GREATER:
                greater();
                break;
            case OP_GREATER_EQUAL:
                greaterEqual();
                break;
            case OP_LESS:
                less();
                break;
            case OP_LESS_EQUAL:
                lessEqual();
                break;
            case OP_ADD:
                add();
                break;
            case OP_SUBTRUCT:
                subtract();
                break;
            case OP_MULTIPLY:
                multiply();
                break;
            case OP_DIVIDE:
                divide();
                break;
            case OP_MOD:
                mod();
                break;
            case OP_NOT:
                push(BOOL_VAL(isFalsey(pop())));
                break;
            case OP_NEGATE:
                if (!IS_NUMBER(peek(0))) {
                    printf("i'll fix it later\n");
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(NUMBER_VAL(-AS_NUMBER(pop())));
                break;
            case OP_RETURN:
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
        }
    }
    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef BINARY_OP
}

void initVM() {
    resetStack();
}

void freeVM() {
    return;
}


InterpretResult interpret(const char *source) {
    CodeVec code;
    initCodeVec(&code);

    int errorCount = compile(source, &code);
    if (errorCount != 0) {
        if (errorCount == 1) 
            printf("Compilation ended with 1 error\n");
        else
            printf("Compilation ended with %d error\n", errorCount);

        freeCodeVec(&code);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.code = &code;
    vm.ip = vm.code->code;

    InterpretResult result = run();

    freeCodeVec(&code);
    return result;
}