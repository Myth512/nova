#ifndef OBJECT_EXCEPTION_H
#define OBJECT_EXCEPTION_H

#include "object.h"

#define IS_EXCEPTION(value)     (value.type == VAL_EXCEPTION)

#define AS_EXCEPTION(value)     ((ObjException*)value.as.object)

#define EXCEPTION_METHODS (ValueMethods) { \
    .init = Exception_Init,                \
    .class = Exception_Class,              \
    .str = Exception_ToStr,                \
    .repr = Exception_ToRepr               \
}

#define ZERO_DIVISON_ERROR_METHODS (ValueMethods) { \
    .init = ZeroDivisionError_Init,                 \
    .class = ZeroDivisionError_Class,               \
    .str = Exception_ToStr,                         \
    .repr = Exception_ToRepr,                       \
}

#define STOP_ITERATION_METHODS (ValueMethods) { \
    .init = StopIteration_Init,                 \
    .class = StopIteration_Class,               \
    .str = Exception_ToStr,                     \
    .repr = Exception_ToRepr,                   \
}

#define NAME_ERROR_METHODS (ValueMethods) { \
    .init = NameError_Init,                 \
    .class = NameError_Class,               \
    .str = Exception_ToStr,                 \
    .repr = Exception_ToRepr,               \
}

typedef struct {
    Obj obj;
    Value value;
} ObjException;

ObjException *allocateException(Value value, ValueType type);

Value createMsgException(char *msg, ValueType type);

Value Exception_Init(Value callee, int argc, Value *argv);

Value Exception_Class(Value value);

int Exception_ToStr(Value value, char *buffer, const size_t size);

int Exception_ToRepr(Value value, char *buffer, const size_t size);

Value ZeroDivisionError_Init(Value callee, int argc, Value *argv);

Value ZeroDivisionError_Class(Value value);

Value StopIteration_Init(Value callee, int argc, Value *argv);

Value StopIteration_Class(Value value);

Value NameError_Init(Value callee, int argc, Value *argv);

Value NameError_Class(Value value);

#endif