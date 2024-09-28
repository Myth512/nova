#include <math.h>
#include <string.h>

#include "vm.h"
#include "debug.h"
#include "object.h"
#include "memory.h"
#include "compiler.h"

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

static inline void concatenate() {
    ObjString *b = AS_STRING(pop());
    ObjString *a = AS_STRING(pop());

    int length = a->length + b->length;

    char *chars = ALLOCATE(char, length + 1);

    memcpy(chars, a->chars, a->length);
    memcpy(chars + a->length, b->chars, b->length);
    chars[length] = '\0';

    ObjString *result = takeString(chars, length);
    push(OBJ_VAL(result));
}

static inline void add() {
    if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
        concatenate();
        return;
    }
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

static inline void equal() {
    Value b = pop();
    Value a = pop();
    push(BOOL_VAL(compareValues(a, b)));
}

static inline void notEqual() {
    Value b = pop();
    Value a = pop();
    push(BOOL_VAL(!compareValues(a, b)));
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
    #define READ_STRING()   AS_STRING(READ_CONSTANT())

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
            case OP_POP:
                pop();
                break;
            case OP_GET_GLOBAL: {
                ObjString *name = READ_STRING();
                Value value;
                if (!tableGet(&vm.globals, name, &value)) {
                    printf("Undefined variable '%s'\n", name->chars);
                }
                push(value);
                break;
            }
            case OP_DEFINE_GLOBAL: {
                ObjString *name = READ_STRING();
                tableSet(&vm.globals, name, pop());
                break;
            }
            case OP_SET_GLOBAL: {
                ObjString *name = READ_STRING();
                if (tableSet(&vm.globals, name, peek(0))) {
                    tableDelete(&vm.globals, name);
                    printf("Undefined variable '%s'\n", name->chars);
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_GET_LOCAL: {
                uint8_t slot = READ_BYTE();
                push(vm.stack[slot]);
                break;
            }
            case OP_SET_LOCAL: {
                uint8_t slot = READ_BYTE();
                vm.stack[slot] = peek(0);
                break;
            }
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
            case OP_PRINT:
                printValue(pop());
                putchar('\n');
                break;
            case OP_RETURN:
                return INTERPRET_OK;
        }
    }
    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef READ_STRING
}

void initVM() {
    resetStack();
    vm.objects = NULL;
    initTable(&vm.globals);
    initTable(&vm.strings);
}

void freeVM() {
    freeObjects();
    initTable(&vm.globals);
    freeTable(&vm.strings);
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