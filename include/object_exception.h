#ifndef OBJECT_EXCEPTION_H
#define OBJECT_EXCEPTION_H

#include "object.h"

#define IS_EXCEPTION(value)     (value.type == VAL_EXCEPTION)

#define AS_EXCEPTION(value)     ((ObjException*)value.as.object)

#define EXCEPTION_METHODS (ValueMethods) { \
    .init = Exception_Init,               \
    .str = Exception_ToStr,               \
    .repr = Exception_ToRepr              \
}

typedef struct {
    Obj obj;
    Value value;
} ObjException;

ObjException *allocateException(Value value);

Value createMsgException(char *msg);

Value Exception_Init(Value callee, int argc, Value *argv);

int Exception_ToStr(Value value, char *buffer, const size_t size);

int Exception_ToRepr(Value value, char *buffer, const size_t size);

#endif