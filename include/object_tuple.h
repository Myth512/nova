#ifndef OBJECT_TUPLE_H
#define OBJECT_TUPLE_H

#include "object.h"
#include "value_methods.h"

#define IS_TUPLE(value)         isObjType(value, OBJ_TUPLE)

#define AS_TUPLE(value)         ((ObjTuple*)value.as.object)

#define TUPLE_METHODS (ValueMethods) { \
    .str = Tuple_ToStr,                \
    .repr = Tuple_ToStr,               \
}

typedef struct {
    Obj obj;
    size_t size;
    Value values[];
} ObjTuple;

ObjTuple* allocateTuple(size_t size);

int Tuple_Index(Value obj, Value value);

int Tuple_ToStr(Value value, char *buffer, size_t size);

#endif