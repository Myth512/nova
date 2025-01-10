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

#define TYPE_ERROR_METHODS (ValueMethods) { \
    .init = TypeError_Init,                 \
    .class = TypeError_Class,               \
    .str = Exception_ToStr,                 \
    .repr = Exception_ToRepr                \
}

#define VALUE_ERROR_METHODS (ValueMethods) { \
    .init = ValueError_Init,                 \
    .class = ValueError_Class,               \
    .str = Exception_ToStr,                  \
    .repr = Exception_ToRepr                 \
}

#define INDEX_ERROR_METHODS (ValueMethods) { \
    .init = IndexError_Init,                 \
    .class = IndexError_Class,               \
    .str = Exception_ToStr,                  \
    .repr = Exception_ToRepr                 \
}

#define KEY_ERROR_METHODS (ValueMethods) { \
    .init = KeyError_Init,                 \
    .class = KeyError_Class,               \
    .str = Exception_ToStr,                \
    .repr = Exception_ToRepr               \
}

#define ATTRIBUTE_ERROR_METHODS (ValueMethods) { \
    .init = AttributeError_Init,                 \
    .class = AttributeError_Class,               \
    .str = Exception_ToStr,                      \
    .repr = Exception_ToRepr                     \
}

#define RUNTIME_ERROR_METHODS (ValueMethods) { \
    .init = RuntimeError_Init,                 \
    .class = RuntimeError_Class,               \
    .str = Exception_ToStr,                    \
    .repr = Exception_ToRepr                   \
}

typedef struct {
    Obj obj;
    Value value;
} ObjException;

ObjException *allocateException(Value value, ValueType type);

Value createException(ValueType type, char *format, ...);

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

Value TypeError_Init(Value callee, int argc, Value *argv);

Value TypeError_Class(Value value);

Value ValueError_Init(Value callee, int argc, Value *argv);

Value ValueError_Class(Value value);

Value IndexError_Init(Value callee, int argc, Value *argv);

Value IndexError_Class(Value value);

Value KeyError_Init(Value callee, int argc, Value *argv);

Value KeyError_Class(Value value);

Value AttributeError_Init(Value callee, int argc, Value *argv);

Value AttributeError_Class(Value value);

Value RuntimeError_Init(Value callee, int argc, Value *argv);

Value RuntimeError_Class(Value value);

#endif