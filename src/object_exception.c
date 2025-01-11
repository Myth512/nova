#include <stdarg.h>

#include "object_exception.h"
#include "object_string.h"
#include "value_methods.h"
#include "vm.h"

ObjException *allocateException(Value value, ValueType type) {
    ObjException *exception = (ObjException*)allocateObject(sizeof(ObjException), type);
    exception->value = value;
    return exception;
}

Value createException(ValueType type, char *format, ...) {
    static char buffer[1024];
    va_list args;
    va_start(args, format);

    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    ObjString *string = copyString(buffer, len);
    ObjException *exception = allocateException(OBJ_VAL(string), type);

    va_end(args);
    return OBJ_VAL(exception);
}

static Value init(int argc, Value *argv, ValueType type) {
    Value value = NONE_VAL;
    if (argc == 1)
        value = argv[0];
    ObjException *exception = allocateException(value, type);
    return OBJ_VAL(exception);
}

Value Exception_Init(Value callee, int argc, Value *argv) {
    return init(argc, argv, VAL_EXCEPTION);
}

Value Exception_Class(Value value) {
    return TYPE_CLASS(exception);
}

int Exception_ToStr(Value value, char *buffer, const size_t size) {
    valueWrite(AS_EXCEPTION(value)->value, buffer, size);
}

int Exception_ToRepr(Value value, char *buffer, const size_t size) {
    size_t bytesLeft = size;
    int bytesWritten = writeToBuffer(buffer, bytesLeft, "%s(", getValueType(value));
    bytesLeft -= movePointer(&buffer, bytesWritten);

    bytesWritten = valueReprWrite(AS_EXCEPTION(value)->value, buffer, bytesLeft);
    bytesLeft -= movePointer(&buffer, bytesWritten);

    writeToBuffer(buffer, bytesLeft, ")");
}

Value ZeroDivisionError_Init(Value callee, int argc, Value *argv) {
    return init(argc, argv, VAL_ZERO_DIVISON_ERROR);
}

Value ZeroDivisionError_Class(Value value) {
    return TYPE_CLASS(zeroDivisionError);
}

Value StopIteration_Init(Value callee, int argc, Value *argv) {
    return init(argc, argv, VAL_STOP_ITERATION);
}

Value StopIteration_Class(Value value) {
    return TYPE_CLASS(stopIteration);
}

Value NameError_Init(Value callee, int argc, Value *argv) {
    return init(argc, argv, VAL_NAME_ERROR);
}

Value NameError_Class(Value value) {
    return TYPE_CLASS(nameError);
}

Value TypeError_Init(Value callee, int argc, Value *argv) {
    return init(argc, argv, VAL_TYPE_ERROR);
}

Value TypeError_Class(Value value) {
    return TYPE_CLASS(typeError);
}

Value ValueError_Init(Value callee, int argc, Value *argv) {
    return init(argc, argv, VAL_VALUE_ERROR);
}

Value ValueError_Class(Value value) {
    return TYPE_CLASS(valueError);
}

Value IndexError_Init(Value callee, int argc, Value *argv) {
    return init(argc, argv, VAL_INDEX_ERROR);
}

Value IndexError_Class(Value value) {
    return TYPE_CLASS(indexError);
}

Value KeyError_Init(Value callee, int argc, Value *argv) {
    return init(argc, argv, VAL_KEY_ERROR);
}

Value KeyError_Class(Value value) {
    return TYPE_CLASS(keyError);
}

Value AttributeError_Init(Value callee, int argc, Value *argv) {
    return init(argc, argv, VAL_ATTRIBUTE_ERROR);
}

Value AttributeError_Class(Value value) {
    return TYPE_CLASS(attributeError);
}

Value RuntimeError_Init(Value callee, int argc, Value *argv) {
    return init(argc, argv, VAL_RUNTIME_ERROR);
}

Value RuntimeError_Class(Value value) {
    return TYPE_CLASS(runtimeError);
}

Value AssertionError_Init(Value callee, int argc, Value *argv) {
    return init(argc, argv, VAL_ASSERTION_ERROR);
}

Value AssertionError_Class(Value value) {
    return TYPE_CLASS(assertionError);
}
