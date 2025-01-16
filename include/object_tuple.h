#ifndef OBJECT_TUPLE_H
#define OBJECT_TUPLE_H

#include "object.h"
#include "value_methods.h"

#define IS_TUPLE(value)         (value.type == VAL_TUPLE) 

#define AS_TUPLE(value)         ((ObjTuple*)value.as.object)

#define TUPLE_METHODS (ValueMethods) { \
    .eq = Tuple_Equal,                 \
    .ne = Tuple_NotEqual,              \
    .gt = Tuple_Greater,               \
    .ge = Tuple_GreaterEqual,          \
    .lt = Tuple_Less,                  \
    .le = Tuple_LessEqual,             \
    .add = Tuple_Add,                  \
    .mul = Tuple_Multiply,             \
    .rmul = Tuple_RightMultiply,       \
    .contains = Tuple_Contains,        \
    .getitem = Tuple_GetItem,          \
    .class = Tuple_Class,              \
    .iter = Tuple_Iter,                \
    .len = Tuple_Len,                  \
    .toBool = Tuple_ToBool,            \
    .str = Tuple_ToStr,                \
    .repr = Tuple_ToStr,               \
}

typedef struct {
    Obj obj;
    size_t size;
    Value values[];
} ObjTuple;

ObjTuple* allocateTuple(size_t size);

Value Tuple_Equal(Value a, Value b);

Value Tuple_NotEqual(Value a, Value b);

Value Tuple_Greater(Value a, Value b);

Value Tuple_GreaterEqual(Value a, Value b);

Value Tuple_Less(Value a, Value b);

Value Tuple_LessEqual(Value a, Value b);

Value Tuple_Add(Value a, Value b);

Value Tuple_Multiply(Value a, Value b);

Value Tuple_RightMultiply(Value a, Value b);

Value Tuple_Contains(Value a, Value b);

Value Tuple_GetItem(Value obj, Value key);

Value Tuple_Class(Value value);

Value Tuple_Iter(Value value);

int Tuple_Index(Value obj, Value value, int start, int end);

long long Tuple_Len(Value value);

bool Tuple_ToBool(Value value);

int Tuple_ToStr(Value value, char *buffer, size_t size);

#endif