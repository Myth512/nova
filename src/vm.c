#include <math.h>
#include <string.h>

#include "vm.h"
#include "debug.h"
#include "object.h"
#include "memory.h"
#include "compiler.h"

#define READ_BYTE()     (*vm.ip++)
#define READ_CONSTANT() (vm.code->constants.values[READ_BYTE()])
#define READ_SHORT()    (vm.ip += 2, (uint16_t)((vm.ip[-2] << 8) | vm.ip[-1]))
#define READ_STRING()   AS_STRING(READ_CONSTANT())

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
        printf("Division by zero\n");
        exit(1);
    }
    return a / b;
}

static inline double modulo(double a, double b) {
    if (b == 0) {
        printf("Division by zero\n");
        exit(1);
    }
    return fmod(a, b);
}

static void arithmetic(double (*function)(double, double)) {
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
        printf("Operands must be numbers\n");
        return;
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

static inline void factorial() {
    if (!IS_NUMBER(peek(0))) {
        printf("Operand must be number\n");
    }

    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(tgamma(a + 1)));
}

static inline void absolute() {
    if (!IS_NUMBER(peek(0))) {
        printf("Operand must be number\n");
    }

    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(fabs(a)));
}

static inline void increment() {
    if (!IS_NUMBER(peek(0))) {
        printf("Operand must be number\n");
    }

    double a = AS_NUMBER(pop());

    push(NUMBER_VAL(a + 1));
}

static inline void decrement() {
    if (!IS_NUMBER(peek(0))) {
        printf("Operand must be number\n");
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
        printf("Operands must be numbers\n");
        return;
    }

    double b = AS_NUMBER(pop());
    double a = AS_NUMBER(pop());
    
    push(BOOL_VAL(function(a, b)));
}

static bool isFalsey(Value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value)) || (IS_NUMBER(value) && AS_NUMBER(value) == 0);
}

static void buildFormattedString() {
    const int bufferSize = 512;
    int size = sizeof(ObjString) + bufferSize;

    ObjString *string = (ObjString*)allocateObject(size, OBJ_STRING);
    string->isInterned = false;
    string->isHashed = false;

    char *buffer = string->chars;
    int partCount = READ_BYTE();
    int stringLength = 0;

    for (int i = 0; i < partCount; i++) {
        Value part = peek(partCount - i - 1);
        int offset = writeValue(part, buffer);
        buffer += offset;
        stringLength += offset;
    }

    string->length = stringLength;

    int newSize = size - (bufferSize - stringLength);

    reallocate(string, size, newSize);

    for (int i = 0; i < partCount; i++)
        pop();

    push(OBJ_VAL(string));
}

static InterpretResult run() {
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
            case OP_FACTORIAL:
                factorial();
                break;
            case OP_ABS:
                absolute();
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
            case OP_BUILD_FSTRING:
                buildFormattedString();
                break;
            case OP_JUMP: {
                uint16_t offset = READ_SHORT();
                vm.ip += offset;
                break;
            }
            case OP_JUMP_IF_FALSE: {
                uint16_t offset = READ_SHORT();
                if (isFalsey(peek(0))) 
                    vm.ip += offset;
                break;
            }
            case OP_JUMP_IF_FALSE_AND_POP: {
                uint16_t offset = READ_SHORT();
                if (isFalsey(peek(0))) 
                    vm.ip += offset;
                pop();
                break;
            }
            case OP_LOOP: {
                uint16_t offset = READ_SHORT();
                vm.ip -= offset;
                break;
            }
            case OP_RETURN:
                return INTERPRET_OK;
        }
    }
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
            printf("Compilation \033[31mfailed\033[0m with 1 error\n");
        else
            printf("Compilation \033[31mfailed\033[0m with %d errors\n", errorCount);

        freeCodeVec(&code);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.code = &code;
    vm.ip = vm.code->code;

    InterpretResult result = run();

    freeCodeVec(&code);
    return result;
}