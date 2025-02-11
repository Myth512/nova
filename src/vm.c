#include <math.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <libgen.h>

#include "vm.h"
#include "debug.h"
#include "value_int.h"
#include "value_methods.h"
#include "object.h"
#include "object_string.h"
#include "object_list.h"
#include "object_tuple.h"
#include "object_dict.h"
#include "object_class.h"
#include "object_instance.h"
#include "object_function.h"
#include "object_exception.h"
#include "object_slice.h"
#include "object_module.h"
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
    if (line != 0)
        printHighlightedPartInCode(frame->closure->function->module->source, line, column, length); 
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
    if (!vm.allowStackPrinting)
        return;
    vm.allowStackPrinting = false;

    printf("%s\t", prefix);
    for (Value *slot = vm.stack; slot < vm.top; slot++) {
        printf("[ ");
        valueRepr(*slot);
        printf(" ]");
    }
    printf("\n");

    vm.allowStackPrinting = true;
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
    Value res = *vm.top;
    #ifdef DEBUG_TRACE_STACK
        printStack("pop");
    #endif
    return res;
}

static Value peek(int distance) {
    return vm.top[-1 - distance];
}

void insert(int distance, Value value) {
    vm.top[-1 - distance] = value;
    #ifdef DEBUG_TRACE_STACK
        printStack("insert");
    #endif
}

int stringIndex(char *keywords[], char *keyword, size_t size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(keywords[i], keyword) == 0)
            return i;
    }
    return -1;
}

void parseArgs(int argc, int kwargc, int arity, char *keywords[], ...) {
    Value args[arity];
    for (int i = 0; i < arity; i++)
        args[i] = UNDEFINED_VAL;

    vm.top -= argc + 2 * kwargc;

    for (int i = 0; i < argc; i++) {
        if (*keywords[i] == '*') {
            size_t size = argc - i;
            ObjTuple *tuple = allocateTuple(size);
            for (int j = 0; j < size; j++) {
                tuple->values[j] = *vm.top;
                vm.top++;
            }
            args[i] = OBJ_VAL(tuple);
            i = argc;
        } else {
            args[i] = *vm.top;
            vm.top++;
        }
    }

    for (int i = 0; i < kwargc; i++) {
        Value name = vm.top[0];
        Value value = vm.top[1];
        vm.top += 2;
        int index = stringIndex(keywords, AS_STRING(name)->chars, arity);
        if (index == -1)
            reportRuntimeError("got an unexpected keyword argument '%s'", AS_STRING(name)->chars);
        if (!IS_UNDEFINED(args[index]))
            reportRuntimeError("got multiple values for argument '%s'", AS_STRING(name)->chars);
        
        args[index] = value;
    }

    va_list args1;
    va_start(args1, arity);

    for (int i = 0; i < arity; i++) {
        Value *value = va_arg(args1, Value*);
        *value = args[i];
    }

    va_end(args1);
}

void loadModule() {
    ObjModule *module = AS_MODULE(peek(0));
    tableSet(&module->globals, OBJ_VAL(copyString("__name__", 0)), OBJ_VAL(module->function->name));
    frame = &vm.frames[vm.frameSize++];
    frame->closure = createClosure(module->function);
    frame->ip = module->function->code.code;
    frame->slots = vm.top;
}

int unloadModule() {
    vm.frameSize--;
    if (!vm.frameSize)
        return 0;
    vm.top = frame->slots;
    frame = &vm.frames[vm.frameSize - 1];
    pop();
    ObjModule *module = AS_MODULE(peek(0));
    return vm.frameSize;
}

void call(ObjClosure *closure, int argc, int kwargc, bool isMethod) {
    if (vm.frameSize == FRAMES_SIZE)
        reportRuntimeError("Stack overflow");

    CallFrame *frame = &vm.frames[vm.frameSize++];
    frame->closure = closure;
    frame->ip = closure->function->code.code;
    frame->isMethod = isMethod;

    int arity = closure->function->arity;
    int defaultStart = closure->function->defaultStart;
    int defaultCount = closure->function->defaults->size;
    int extraArgs = closure->function->extraArgs;
    int extraKwargs = closure->function->extraKwargs;

    if (argc > arity && extraArgs == -1 && extraKwargs == -1)
        reportArityError(arity - defaultCount, arity, argc);

    Value args[arity];
    for (int i = 0; i < arity; i++)
        args[i] = UNDEFINED_VAL;

    vm.top -= argc + 2 * kwargc;

    int i = 0;

    for (; i < argc; i++) {
        if (extraArgs != -1 && i >= extraArgs)
            break;
        args[i] = *vm.top;
        vm.top++;
    }

    if (i <= extraArgs) {
        size_t size = argc - i;
        if (size < 0)
            size = 0;
        ObjTuple *tuple = allocateTuple(size);

        for (int i = 0; i < size; i++) {
            tuple->values[i] = *vm.top;
            vm.top++;
        }

        args[extraArgs] = OBJ_VAL(tuple);
    }

    ObjDict *kwargs = NULL;
    if (extraKwargs != -1)
        kwargs = allocateDict();
    Value kwargsVal;
    if (kwargs != NULL) {
        kwargsVal = OBJ_VAL(kwargs);
        args[extraKwargs] = kwargsVal;
    }

    for (int i = 0; i < kwargc; i++) {
        Value name = vm.top[0];
        Value value = vm.top[1];
        vm.top += 2;
        int index = Tuple_Index(OBJ_VAL(closure->function->localNames), name, isMethod, arity);
        if (index == -1) {
            if (kwargs == NULL)
                reportRuntimeError("got an unexpected keyword argument '%s'", AS_STRING(name)->chars);
            Dict_SetItem(kwargsVal, name, value);
        }
        if (index > 0 && !IS_UNDEFINED(args[index]))
            reportRuntimeError("got multiple values for argument '%s'", AS_STRING(name)->chars);
        
        args[index] = value;
    }

    for (int i = argc - defaultStart; i < defaultCount; i++) {
        if (i < 0)
            continue;
        if (IS_UNDEFINED(args[defaultStart + i]))
            args[defaultStart + i] = closure->function->defaults->values[i];
    }

    for (int i = 0; i < arity; i++) {
        if (IS_UNDEFINED(args[i]))
            reportRuntimeError("missing required argument: '%s'", AS_STRING(closure->function->localNames->values[i])->chars);
    }

    vm.top -= argc + 2 * kwargc;
    frame->slots = vm.top;

    for (int i = 0; i < arity; i++) {
        *vm.top = args[i];
        vm.top++;
    }

    for (int i = 0; i < closure->function->localNames->size - arity; i++)
        push(UNDEFINED_VAL);
}

static void defineNative(const char *name, NativeFn function) {
    push(STRING_VAL(copyString(name, (int)strlen(name))));
    ObjNative *native = createNative(function, name);
    push(NATIVE_VAL(native));
    tableSet(&vm.builtin, vm.stack[0], vm.stack[1]);
    pop();
    pop();
}

static void defineNatives() {
    // defineNative("time", Py_Time);
    defineNative("print", Py_Print);
    // defineNative("sqrt", sqrtNative);
    // defineNative("min", minNative);
    // defineNative("max", maxNative);
    defineNative("id", Py_Id);
    defineNative("len", Py_Len);
    defineNative("repr", Py_Repr);
    defineNative("input", Py_Input);
    defineNative("hex", Py_Hex);
    defineNative("isinstance", Py_IsInstance);
    defineNative("iter", Py_Iter);
    defineNative("next", Py_Next);
}

static ObjNativeClass* defineNativeClass(const char *name, ValueType type, ValueType super) {
    ObjString *n = copyString(name, strlen(name));
    ObjNativeClass *class = createNativeClass(n, type, super);
    tableSet(&vm.builtin, OBJ_VAL(n), OBJ_VAL(class));
    return class;
}

static ObjNativeClass* createNativeclass(const char *name, ValueType type, ValueType super) {
    ObjString *n = copyString(name, strlen(name));
    ObjNativeClass *class = createNativeClass(n, type, super);
    return class;
}

static void defineNativeTypes() {
    vm.types.object = defineNativeClass("object", VAL_OBJECT, VAL_UNDEFINED);
    vm.types.none = createNativeclass("NoneType", VAL_NONE, VAL_OBJECT);
    vm.types.int_ = defineNativeClass("int", VAL_INT, VAL_OBJECT);
    vm.types.bool_ = defineNativeClass("bool", VAL_BOOL, VAL_INT);
    vm.types.float_ = defineNativeClass("float", VAL_FLOAT, VAL_OBJECT);
    vm.types.type = defineNativeClass("type", VAL_TYPE, VAL_OBJECT);
    vm.types.str = defineNativeClass("str", VAL_STRING, VAL_OBJECT);
    vm.types.strIterator = createNativeclass("str_iterator", VAL_STRING_ITERATOR, VAL_OBJECT);
    vm.types.list = defineNativeClass("list", VAL_LIST, VAL_OBJECT);
    vm.types.listIterator = createNativeclass("list_iterator", VAL_LIST_ITERATOR, VAL_OBJECT);
    vm.types.tuple = defineNativeClass("tuple", VAL_TUPLE, VAL_OBJECT);
    vm.types.tupleIterator = createNativeclass("tuple_iterator", VAL_TUPLE_ITERATOR, VAL_OBJECT);
    vm.types.dict = defineNativeClass("dict", VAL_DICT, VAL_OBJECT);
    vm.types.dictIterator = createNativeclass("dict_iterator", VAL_TUPLE_ITERATOR, VAL_OBJECT);
    vm.types.exception = defineNativeClass("Exception", VAL_EXCEPTION, VAL_OBJECT);
    vm.types.zeroDivisionError = defineNativeClass("ZeroDivisionError", VAL_ZERO_DIVISON_ERROR, VAL_EXCEPTION);
    vm.types.stopIteration = defineNativeClass("StopIteration", VAL_STOP_ITERATION, VAL_EXCEPTION);
    vm.types.nameError = defineNativeClass("NameError", VAL_NAME_ERROR, VAL_EXCEPTION);
    vm.types.typeError = defineNativeClass("TypeError", VAL_TYPE_ERROR, VAL_EXCEPTION);
    vm.types.valueError = defineNativeClass("ValueError", VAL_VALUE_ERROR, VAL_EXCEPTION);
    vm.types.indexError = defineNativeClass("IndexError", VAL_INDEX_ERROR, VAL_EXCEPTION);
    vm.types.keyError = defineNativeClass("KeyError", VAL_KEY_ERROR, VAL_EXCEPTION);
    vm.types.attributeError = defineNativeClass("AttributeError", VAL_ATTRIBUTE_ERROR, VAL_EXCEPTION);
    vm.types.runtimeError = defineNativeClass("RuntimeError", VAL_RUNTIME_ERROR, VAL_EXCEPTION);
    vm.types.assertionError = defineNativeClass("AssertionError", VAL_ASSERTION_ERROR, VAL_EXCEPTION);
    vm.types.notImplementedError = defineNativeClass("NotImplementedError", VAL_NOT_IMPLEMENTED_ERROR, VAL_EXCEPTION);
    vm.types.super = defineNativeClass("super", VAL_SUPER, VAL_OBJECT);
    vm.types.range = defineNativeClass("range", VAL_RANGE, VAL_OBJECT);
    vm.types.rangeIterator = createNativeclass("range_iterator", VAL_RANGE_ITERATOR, VAL_OBJECT);
    vm.types.slice = defineNativeClass("slice", VAL_SLICE, VAL_OBJECT);
    vm.types.notImplementedType = createNativeclass("NotImplementedType", VAL_NOT_IMPLEMENTED, VAL_OBJECT);
}

static void callValue(Value callee, int argc, int kwargc) {
    Value res = valueCall(callee, argc, kwargc, vm.top);
    frame = &vm.frames[vm.frameSize - 1];
    if (isInstance(res, TYPE_CLASS(exception))) {
        push(res);
        raise();
    }
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
    nameTableSet(&class->methods, name, method);
    pop();
}

static void buildFormattedString() {
    const size_t bufferSize = 512;
    size_t spaceLeft = bufferSize;
    int size = sizeof(ObjString) + bufferSize;

    ObjString *string = (ObjString*)allocateObject(size, VAL_STRING);
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

    push(STRING_VAL(string));
}

static void buildList() {
    size_t size = READ_BYTE();
    ObjList *list = allocateList(size);

    for (int i = 0; i < size; i++) {
        Value value = peek(size - i - 1);
        list->vec.values[i] = value;
    }
    
    for (int i = 0; i < size; i++)
        pop();
    
    push(OBJ_VAL(list));
}

static void buildTuple() {
    size_t size = READ_BYTE();
    ObjTuple *tuple = allocateTuple(size);

    for (int i = 0; i < size; i++) {
        Value value = peek(size - i - 1);
        tuple->values[i] = value;
    }
    
    for (int i = 0; i < size; i++)
        pop();
    
    push(OBJ_VAL(tuple));
}

static void buildDict() {
    size_t size = READ_BYTE();
    ObjDict *dict = allocateDict();
    Value res = OBJ_VAL(dict);

    for (int i = 0; i < size; i++){
        Value value = peek((size - i) * 2 - 2);
        Value key = peek((size - i) * 2 - 1);
        Dict_SetItem(res, key, value);
    }

    for (int i = 0; i < 2 * size; i++)
        pop();

    push(res);
}

static void buildSlice() {
    Value step = pop();
    Value stop = pop();
    Value start = pop();
    push(OBJ_VAL(allocateSlice(start, stop, step)));
}

static bool return_() {
    Value result = pop();
    
    closeUpvalues(frame->slots);
    // if (strcmp(frame->closure->function->name->chars, "_init_") == 0)
    //     AS_INSTANCE(result)->isInitiazed = true;

    vm.frameSize--;
    frame->slots;
    vm.top = frame->slots - !frame->isMethod;
    frame = &vm.frames[vm.frameSize - 1];

    push(result);
    return !vm.frameSize;
}

void raise() {
    Value exception = pop();

    while (frame->exceptPointer == 0 && vm.frameSize > 1) {
        return_();
    }

    if (frame->exceptPointer == 0) {
        fprintf(stderr, "%s: ", getValueType(exception));
        valuePrint(exception);
        printf("\n");
        printErrorInCode();
        exit(1);
    }
    vm.top = frame->slots + frame->closure->function->localNames->size;
    push(exception);
    frame->ip = frame->exceptAddr[frame->exceptPointer - 1];
}

void raiseIfException() {
    if (isInstance(peek(0), TYPE_CLASS(exception)))
        raise();
}

static void binary(Value (*func)(Value, Value)) {
    Value b = pop();
    Value a = pop();
    push(func(a, b));
    raiseIfException();
}

static void unary(Value (*func)(Value)) {
    Value a = pop();
    Value res = func(a);
    push(func(a));
    raiseIfException();
}

static void getLocal() {
    uint8_t slot = READ_BYTE();
    Value value = frame->slots[slot];

    if (IS_UNDEFINED(value)) {
        char *name = AS_STRING(frame->closure->function->localNames->values[slot])->chars;
        push(createException(VAL_NAME_ERROR, "name '%s' is not defined", name));
        raise();
    } else {
        push(frame->slots[slot]);
    }
}

static void setLocal() {
    uint8_t slot = READ_BYTE();
    frame->slots[slot] = peek(0);
}

static void delLocal() {
    uint8_t slot = READ_BYTE();
    if (IS_UNDEFINED(frame->slots[slot])) {
        char *name = AS_STRING(frame->closure->function->localNames->values[slot])->chars;
        push(createException(VAL_NAME_ERROR, "name '%s' is not defined", name));
        raise();
    } else {
        frame->slots[slot] = UNDEFINED_VAL; 
    }
}

static void getItem(bool popValues) {
    Value key;
    Value object;
    
    if (popValues) {
        key = pop();
        object = pop();
    } else {
        key = peek(0);
        object = peek(1);
    }

    push(valueGetItem(object, key));
    raiseIfException();
}

static void setItem() {
    Value value = pop();
    Value key = pop();
    Value object = peek(0);
    Value res = valueSetItem(object, key, value);

    if (isInstance(res, TYPE_CLASS(exception))) {
        push(res);
        raise();
    }
}

static void delItem() {
    Value key = pop();
    Value object = peek(0);

    Value res = valueDelItem(object, key);
    if (isInstance(res, TYPE_CLASS(exception))) {
        push(res);
        raise();
    }
}

static void getAttrtibute() {
    Value obj = pop();
    ObjString *name = READ_STRING();
    Value result = valueGetAttribute(obj, name);
    push(result);
    raiseIfException();
}

static void setAttribute() {
    Value obj = peek(1);
    ObjString *name = READ_STRING();
    Value value = pop();
    Value res = valueSetAttribute(obj, name, value);

    if (isInstance(res, TYPE_CLASS(exception))) {
        push(res);
        raise();
    }
}

static void delAttribute() {
    Value obj = pop();
    ObjString *name = READ_STRING();
    Value res = valueDelAttribute(obj, name);

    if (isInstance(res, TYPE_CLASS(exception))) {
        push(res);
        raise();
    }
}

static Value run() {
    frame = &vm.frames[vm.frameSize - 1];
    while (true) {

        #ifdef DEBUG_TRACE_EXECUTION
            if (vm.allowStackPrinting)
                printInstruction(&frame->closure->function->code, (int)(frame->ip - frame->closure->function->code.code));
        #endif

        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT:
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            case OP_NONE:
                push(NONE_VAL);
                break;
            case OP_POP:
                pop();
                break;
            case OP_GET_GLOBAL: {
                ObjString *name = READ_STRING();
                Value value = tableGet(&frame->closure->function->module->globals, OBJ_VAL(name));
                if (!IS_UNDEFINED(value)) {
                    push(value);
                    break;
                }
                value = tableGet(&vm.builtin, OBJ_VAL(name));
                if (!IS_UNDEFINED(value)) {
                    push(value);
                    break;
                }
                push(createException(VAL_NAME_ERROR, "name '%s' is not defined", name->chars));
                raise();
                break;
            }
            case OP_SET_GLOBAL: {
                ObjString *name = READ_STRING();
                tableSet(&frame->closure->function->module->globals, OBJ_VAL(name), peek(0));
                break;
            }
            case OP_DEL_GLOBAL: {
                ObjString *name = READ_STRING();
                Value value = tableDelete(&frame->closure->function->module->globals, OBJ_VAL(name));
                if (IS_UNDEFINED(value)) {
                    push(createException(VAL_NAME_ERROR, "name '%s' is not defined", name->chars));
                    raise();
                }
                break;
            }
            case OP_GET_LOCAL:
                getLocal();
                break;
            case OP_SET_LOCAL:
                setLocal();
                break;
            case OP_DEL_LOCAL:
                delLocal();
                break;
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
            case OP_GET_ITEM:
                getItem(true);
                break;
            case OP_GET_ITEM_NO_POP:
                getItem(false);
                break;
            case OP_SET_ITEM:
                setItem();
                break;
            case OP_DEL_ITEM:
                delItem();
                break; 
            case OP_FALSE:
                push(BOOL_VAL(false));
                break;
            case OP_TRUE:
                push(BOOL_VAL(true));
                break;
            case OP_EQUAL:
                binary(valueEqual);
                break;
            case OP_NOT_EQUAL:
                binary(valueNotEqual);
                break;
            case OP_GREATER:
                binary(valueGreater);
                break;
            case OP_GREATER_EQUAL:
                binary(valueGreaterEqual);
                break;
            case OP_LESS:
                binary(valueLess);
                break;
            case OP_LESS_EQUAL:
                binary(valueLessEqual);
                break;
            case OP_ADD:
                binary(valueAdd); 
                break;
            case OP_SUBTRUCT:
                binary(valueSubtract);
                break;
            case OP_MULTIPLY:
                binary(valueMultiply);
                break;
            case OP_POWER:
                binary(valuePower);
                break;
            case OP_TRUE_DIVIDE:
                binary(valueTrueDivide);
                break;
            case OP_FLOOR_DIVIDE:
                binary(valueFloorDivide);
                break;
            case OP_MOD:
                binary(valueModulo);
                break;
            case OP_POSITIVE:
                unary(valuePositive);
                break;
            case OP_NEGATIVE:
                unary(valueNegative);
                break;
            case OP_BITWISE_AND:
                binary(valueAnd);
                break;
            case OP_BITWISE_XOR:
                binary(valueXor);
                break;
            case OP_BITWISE_OR:
                binary(valueOr);
                break;
            case OP_LEFT_SHIFT:
                binary(valueLeftShift);
                break;
            case OP_RIGHT_SHIFT:
                binary(valueRightShift);
                break;
            case OP_INVERT:
                unary(valueInvert);
                break;
            case OP_NOT:
                push(BOOL_VAL(!valueToBool(pop())));
                break;
            case OP_CONTAINS:
                binary(valueContains);
                break;
            case OP_BUILD_ITERATOR:
                unary(valueIter);
                break;
            case OP_JUMP_NEXT: {
                uint16_t offset = READ_SHORT();
                Value tmp = valueNext(peek(0));
                if (isInstance(tmp, TYPE_CLASS(stopIteration)))
                    frame->ip += offset;
                else 
                    push(tmp);
                break;
            }
            case OP_IS_INSTANCE: {
                Value a = pop();
                Value b = peek(0);
                push(BOOL_VAL(isInstance(b, a)));
                break;
            }
            case OP_IS:
                binary(valueIs);
                break;
            case OP_BUILD_FSTRING:
                buildFormattedString();
                break;
            case OP_BUILD_LIST:
                buildList();
                break;
            case OP_BUILD_TUPLE:
                buildTuple();
                break;
            case OP_BUILD_DICT:
                buildDict();
                break;
            case OP_BUILD_SLICE:
                buildSlice();
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
                if (valueToBool(pop())) 
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
            case OP_SETUP_TRY: {
                uint16_t offset = READ_SHORT();
                frame->exceptAddr[frame->exceptPointer++] = frame->ip + offset;
                break;
            }
            case OP_END_TRY:
                frame->exceptPointer--;
                break;
            case OP_RAISE: {
                if (!isInstance(peek(0), TYPE_CLASS(exception)))
                    push(createException(VAL_RUNTIME_ERROR, "No active exception to reraise"));
                raise();
                break;
            }
            case OP_ASSERT: {
                Value value = pop();
                if (!valueToBool(pop())) {
                    if (IS_NONE(value))
                        push(createException(VAL_ASSERTION_ERROR, ""));
                    else
                        push(OBJ_VAL(allocateException(value, VAL_ASSERTION_ERROR)));
                    raise();
                }
                break;
            }
            case OP_LOAD_MODULE:
                loadModule();
                break;
            case OP_UNLOAD_MODULE:
                if (!unloadModule())
                    return NONE_VAL;
                break;
            case OP_CALL: {
                int argc = READ_BYTE();
                int kwargc = READ_BYTE();
                callValue(peek(argc + 2*kwargc), argc, kwargc);
                break;
            }
            case OP_CLOSURE: {
                ObjFunction *function = AS_FUNCTION(READ_CONSTANT());
                Value defaults = pop();
                function->defaults = AS_TUPLE(defaults);
                ObjClosure *closure = createClosure(function);
                push(CLOSURE_VAL(closure));
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
            case OP_CLASS: {
                ObjString *name = READ_STRING();
                Value super = pop();
                push(OBJ_VAL(createClass(name, super)));
                break;
            }
            case OP_METHOD:
                defineMethod(READ_STRING());
                break;
            case OP_GET_ATTRIBUTE:
                getAttrtibute();
                break;
            case OP_SET_ATTRIBUTE:
                setAttribute();
                break;
            case OP_DEL_ATTRIBUTE:
                delAttribute();
                break;
            case OP_RETURN: {
                if (return_())
                    return pop();
                break;
            }
        }
    }
}

Value callNovaValue(Value callee, int argc) {
    // callValue(callee, argc);
    // return run();
}

OptValue callNovaMethod(Value obj, ObjString *methodName) {
    // OptValue method = valueGetField(obj, methodName);
    // if (method.hasValue) {
    //     push(NIL_VAL);
    //     Value value = callNovaValue(method.value, 0);;
    //     return (OptValue){.hasValue=true, .value=value};
    // }
    // return (OptValue){.hasValue=false};
}

OptValue callNovaMethod1arg(Value obj, ObjString *methodName, Value arg) {
    // OptValue method = valueGetField(obj, methodName);
    // if (method.hasValue) {
    //     push(NIL_VAL);
    //     push(arg);
    //     Value value = callNovaValue(method.value, 1);
    //     return (OptValue){.hasValue=true, .value=value};
    // }
    // return (OptValue){.hasValue=false};
}

OptValue callNovaMethod2args(Value obj, ObjString *methodName, Value arg1, Value arg2) {
    // OptValue method = valueGetField(obj, methodName);
    // if (method.hasValue) {
    //     push(NIL_VAL);
    //     push(arg1);
    //     push(arg2);
    //     Value value = callNovaValue(method.value, 1);
    //     return (OptValue){.hasValue=true, .value=value};
    // }
    // return (OptValue){.hasValue=false};
}

void initMagicStrings() {
    vm.magicStrings.init = copyString("__init__", 8);
    vm.magicStrings.add = copyString("__add__", 7);
    vm.magicStrings.radd = copyString("__radd__", 8);
    vm.magicStrings.sub = copyString("__sub__", 7);
    vm.magicStrings.rsub = copyString("__rsub__", 8);
    vm.magicStrings.neg = copyString("__neg__", 7);
    vm.magicStrings.mul = copyString("__mul__", 7);
    vm.magicStrings.rmul = copyString("__rmul__", 8);
    vm.magicStrings.div = copyString("__truediv__", 11);
    vm.magicStrings.rdiv = copyString("__rtruediv__", 12);
    // TODO
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
}

void initPath(const char *scriptPath) {
    char *buffer = malloc(256);
    strncpy(buffer, scriptPath, 256);
    char *directory = dirname(buffer);
    vm.path = directory;
    setBasePath(directory);
}

void initVM(const char *scriptPath) {
    vm.allowStackPrinting = false;
    resetStack();
    vm.objects = NULL;
    vm.bytesAllocated = 0;
    vm.nextGC = 1024 * 1024;
    initPath(scriptPath);
    tableInit(&vm.builtin);
    initMagicStrings();
    defineNatives();
    defineNativeTypes();
    tableSet(&vm.builtin, OBJ_VAL(copyString("NotImplemented", 0)), NOT_IMPLEMENTED_VAL);
    vm.allowStackPrinting = true;
}

void freeVM() {
    freeObjects();
    return;
}

InterpretResult interpret(const char *source, const char *path) {
    ObjModule *module = compile(source, path, "__main__");
    if (module == NULL)
        return INTERPRET_COMPILE_ERROR;
    push(OBJ_VAL(module));
    loadModule();

    #ifdef DEBUG_DO_NOT_EXECUTE
        return INTERPRET_OK;
    #endif

    run();
    return INTERPRET_OK;
}