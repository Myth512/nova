#include <math.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#include "vm.h"
#include "debug.h"
#include "object.h"
#include "object_string.h"
#include "object_array.h"
#include "object_class.h"
#include "object_function.h"
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

void printErrorInCode() {
    int index = frame->ip - frame->closure->function->code.code - 1;
    int line = frame->closure->function->code.lines[index];
    int column = frame->closure->function->code.columns[index];
    int length = frame->closure->function->code.lengths[index];
    printHighlightedPartInCode(vm.source, line, column, length); 
}

void reportRuntimeError(const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "\033[31mRuntime Error\033[0m: ");
    vprintf(format, args);
    printf("\n");
    printErrorInCode();
    exit(1);
}

void reportArityError(int min, int max, int got) {
    if (min == max)
        reportRuntimeError("Expect exactly %d arguments, but got %d", min, got);
    else
        reportRuntimeError("Expect from %d to %d arguments, but got %d", min, max, got);
    printErrorInCode();
    exit(1);
}

void operatorNotImplemented(char *operator, Value a, Value b) {
    reportRuntimeError("Operator '%s' is not implemented for %s and %s", operator, decodeValueType(a), decodeValueType(b));
    printErrorInCode();
    exit(1);
}

void operatorNotImplementedUnary(char *operator, Value a) {
    reportRuntimeError("Operator '%s' is not implemented for %s", operator, decodeValueType(a));
    printErrorInCode();
    exit(1);
}

void functionNotImplemented(char *function, Value a) {
    reportRuntimeError("Function '%s' is not implemented for %s", function, decodeValueType(a));
    printErrorInCode();
    exit(1);
}

static void printStack(const char *prefix) {
    static bool flag = false;
    if (flag)
        return;
    
    flag = true;
    printf("%s\t", prefix);
    for (Value *slot = vm.stack; slot < vm.top; slot++) {
        printf("[ ");
        valuePrint(*slot);
        printf(" ]");
    }
    printf("\n");

    flag = false;
}

static void resetStack() {
    vm.top = vm.stack;
    vm.frameSize = 0;
    vm.openUpvalues = NULL;
}

void push(Value value) {
    *vm.top = value;
    vm.top++;
    #ifdef DEBUG_TRACE_STACK
        printStack("push");
    #endif
}

Value pop() {
    vm.top--;
    #ifdef DEBUG_TRACE_STACK
        printStack("pop");
    #endif
    return *vm.top;
}

static Value peek(int distance) {
    return vm.top[-1 - distance];
}

static void insert(int distance, Value value) {
    vm.top[-1 - distance] = value;
    #ifdef DEBUG_TRACE_STACK
        printStack("insert");
    #endif
}

static bool call(ObjClosure *closure, int argc) {
    if (argc < closure->function->minArity || argc > closure->function->maxArity)
        reportArityError(closure->function->minArity, closure->function->maxArity, argc);

    if (vm.frameSize == FRAMES_SIZE)
        reportRuntimeError("Stack overflow");

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
    defineNative("len", novaLen);
    defineNative("addr", novaAddr);
    defineNative("bool", novaBool);
    defineNative("int", novaInt);
    defineNative("float", novaFloat);
    defineNative("str", novaStr);
    defineNative("input", novaInput);
}

static bool callValueInternal(Value callee, int argc) {
    if (IS_OBJ(callee)) {
        switch (OBJ_TYPE(callee)) {
            case OBJ_METHOD: {
                ObjMethod *method = AS_METHOD(callee);
                insert(argc, method->reciever);
                return call(method->method, argc);
            }
            case OBJ_NATIVE_METHOD: {
                ObjNativeMethod *method = AS_NATIVE_METHOD(callee);
                insert(argc, method->reciever);
                NativeFn native = method->method;
                Value result = native(argc, vm.top - argc - 1);
                vm.top -= argc + 1;
                push(result);
                return true;
            }
            case OBJ_CLASS: {
                ObjClass *class = AS_CLASS(callee);
                insert(argc, OBJ_VAL(createInstance(class)));
                Value initializer;
                if (tableGet(&class->methods, vm.magicStrings.init, &initializer)) {
                    return call(AS_CLOSURE(initializer), argc);
                } else if (argc != 0) {
                    reportRuntimeError("Expect 0 arguments but got %d", argc);
                    return false;
                }
                return true;
            }
            case OBJ_CLOSURE: {
                ObjClosure *closure = AS_CLOSURE(callee);
                int minArity = closure->function->minArity;
                int maxArity = closure->function->maxArity;
                for (int i = argc; i < maxArity; i++)
                    push(closure->function->defaults->vec.values[i - minArity]);
                return call(AS_CLOSURE(callee), argc < maxArity ? maxArity : argc);
            }
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
    return false;
}

static bool callValue(Value callee, int argc) {
    bool res = callValueInternal(callee, argc);
    frame = &vm.frames[vm.frameSize - 1];
    return res;
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
        int bytesWritten = valueWrite(part, buffer, spaceLeft);
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
        array->vec.values[i] = value;
    }
    
    for (int i = 0; i < size; i++)
        pop();
    
    push(OBJ_VAL(array));
}

static void arithmetic(Value (*func)(Value, Value)) {
    Value b = pop();
    Value a = pop();
    push(func(a, b));
}

static void equality(bool (*func)(Value, Value)) {
    Value b = pop();
    Value a = pop();
    push(BOOL_VAL(func(a, b)));
}

static void unary(Value (*func)(Value)) {
    Value a = pop();
    push(func(a));
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

    push(valueGetAt(object, key));
}

static void setAt() {
    Value value = pop();
    Value key = pop();
    Value object = peek(0);

    valueSetAt(object, key, value);
}

static void getProperty() {
    Value obj = pop();
    ObjString *name = READ_STRING();
    OptValue result = valueGetField(obj, name);
    if (result.hasValue)
        push(result.value);
    else
        reportRuntimeError("Object of %s does not have field %s", decodeValueType(obj), name->chars);
}

static void setProperty() {
    Value obj = peek(1);
    ObjString *name = READ_STRING();
    Value value = pop();
    valueSetField(obj, name, value);
}

static Value run() {
    frame = &vm.frames[vm.frameSize - 1];
    int startFrame = vm.frameSize - 1;
    while (true) {

        #ifdef DEBUG_TRACE_EXECUTION
            if (strcmp(frame->closure->function->name->chars, "_str_"))
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
                equality(valueEqual);
                break;
            case OP_NOT_EQUAL:
                equality(valueNotEqual);
                break;
            case OP_GREATER:
                equality(valueGreater);
                break;
            case OP_GREATER_EQUAL:
                equality(valueGreaterEqual);
                break;
            case OP_LESS:
                equality(valueLess);
                break;
            case OP_LESS_EQUAL:
                equality(valueLessEqual);
                break;
            case OP_INCREMENT:
                unary(valueIncrement);
                break; 
            case OP_DECREMENT:
                unary(valueDecrement);
                break;
            case OP_ADD:
                arithmetic(valueAdd); 
                break;
            case OP_SUBTRUCT:
                arithmetic(valueSubtract);
                break;
            case OP_MULTIPLY:
                arithmetic(valueMultiply);
                break;
            case OP_DIVIDE:
                arithmetic(valueDivide);
                break;
            case OP_MOD:
                arithmetic(valueModulo);
                break;
            case OP_POWER:
                arithmetic(valuePower);
                break;
            case OP_NOT:
                push(BOOL_VAL(!valueToBool(pop())));
                break;
            case OP_NEGATE:
                unary(valueNegate);
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
                if (valueToBool(peek(0)))
                    frame->ip += offset;
                break;
            }
            case OP_JUMP_TRUE_POP: {
                uint16_t offset = READ_SHORT();
                if (!valueToBool(pop())) 
                    frame->ip += offset;
                break;
            }
            case OP_JUMP_FALSE: {
                uint16_t offset = READ_SHORT();
                if (!valueToBool(peek(0))) 
                    frame->ip += offset;
                break;
            }
            case OP_JUMP_FALSE_POP: {
                uint16_t offset = READ_SHORT();
                if (!valueToBool(pop())) 
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
                if (valueToBool(pop()))
                    frame->ip -= offset;
                break;
            }
            case OP_CALL: {
                int argc = READ_BYTE();
                if (!callValue(peek(argc), argc))
                    return NIL_VAL;
                break;
            }
            case OP_CLOSURE: {
                ObjFunction *function = AS_FUNCTION(READ_CONSTANT());
                Value defaults = pop();
                function->defaults = AS_ARRAY(defaults);
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
                if (strcmp(frame->closure->function->name->chars, "_init_") == 0)
                    AS_INSTANCE(result)->isInitiazed = true;
                vm.frameSize--;

                vm.top = frame->slots;
                frame = &vm.frames[vm.frameSize - 1];
                if (vm.frameSize == startFrame) {
                    return result;
                } else {
                    push(result);
                }
                break;
            }
        }
    }
}

Value callNovaValue(Value callee, int argc) {
    callValue(callee, argc);
    return run();
}

OptValue callNovaMethod(Value obj, ObjString *methodName, int argc) {
    OptValue method = valueGetField(obj, methodName);
    if (method.hasValue) {
        push(NIL_VAL);
        Value value = callNovaValue(method.value, argc);;
        return (OptValue){.hasValue=true, .value=value};
    }
    return (OptValue){.hasValue=false};
}

OptValue callNovaMethod1arg(Value obj, ObjString *methodName, Value arg) {
    OptValue method = valueGetField(obj, methodName);
    if (method.hasValue) {
        push(NIL_VAL);
        push(arg);
        Value value = callNovaValue(method.value, 1);
        return (OptValue){.hasValue=true, .value=value};
    }
    return (OptValue){.hasValue=false};
}

OptValue callNovaMethod2args(Value obj, ObjString *methodName, Value arg1, Value arg2) {
    OptValue method = valueGetField(obj, methodName);
    if (method.hasValue) {
        push(NIL_VAL);
        push(arg1);
        push(arg2);
        Value value = callNovaValue(method.value, 1);
        return (OptValue){.hasValue=true, .value=value};
    }
    return (OptValue){.hasValue=false};
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
    vm.magicStrings.bool_ = copyString("_bool_", 6);
    vm.magicStrings.int_ = copyString("_int_", 5);
    vm.magicStrings.float_ = copyString("_float_", 7);
    vm.magicStrings.str = copyString("_str_", 5);
    vm.magicStrings.unsupported = copyString("unsupported", 12);
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
    function->name = copyString("script", 7);
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
    run();
    return INTERPRET_OK;
}