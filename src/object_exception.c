#include "object_exception.h"
#include "object_string.h"
#include "value_methods.h"
#include "vm.h"

ObjException *allocateException(Value value, ValueType type) {
    ObjException *exception = (ObjException*)allocateObject(sizeof(ObjException), type);
    exception->value = value;
    return exception;
}

Value createMsgException(char *msg, ValueType type) {
    ObjString *string = copyString(msg, 0);
    ObjException *exception = allocateException(OBJ_VAL(string), type);
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