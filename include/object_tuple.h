#ifndef OBJECT_TUPLE_H
#define OBJECT_TUPLE_H

#include "object.h"
#include "value_methods.h"

#define IS_TUPLE(value)         (value.type == VAL_TUPLE) 

#define AS_TUPLE(value)         ((ObjTuple*)value.as.object)

#define TUPLE_METHODS (ValueMethods) { \
    .contains = Tuple_Contains,        \
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

Value Tuple_Contains(Value a, Value b);

Value Tuple_Class(Value value);

Value Tuple_Iter(Value value);

int Tuple_Index(Value obj, Value value, int start, int end);

long long Tuple_Len(Value value);

bool Tuple_ToBool(Value value);

int Tuple_ToStr(Value value, char *buffer, size_t size);

#endif