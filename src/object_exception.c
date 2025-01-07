#include "object_exception.h"
#include "object_string.h"
#include "value_methods.h"

ObjException *allocateException(Value value) {
    ObjException *exception = (ObjException*)allocateObject(sizeof(ObjException), VAL_EXCEPTION);
    exception->value = value;
    return exception;
}

Value createMsgException(char *msg) {
    ObjString *string = copyString(msg, 0);
    ObjException *exception = allocateException(OBJ_VAL(string));
    return OBJ_VAL(exception);
}

Value Exception_Init(Value callee, int argc, Value *argv) {
    Value value = NONE_VAL;
    if (argc == 1)
        value = argv[0];
    ObjException *exception = allocateException(value);
    return OBJ_VAL(exception);
}

int Exception_ToStr(Value value, char *buffer, const size_t size) {
    valueWrite(AS_EXCEPTION(value)->value, buffer, size);
}

int Exception_ToRepr(Value value, char *buffer, const size_t size) {
    size_t bytesLeft = size;
    int bytesWritten = writeToBuffer(buffer, size, "Exception(");
    bytesLeft -= movePointer(&buffer, bytesWritten);

    bytesWritten = valueReprWrite(AS_EXCEPTION(value)->value, buffer, size);
    bytesLeft -= movePointer(&buffer, bytesWritten);

    writeToBuffer(buffer, size, ")");
}