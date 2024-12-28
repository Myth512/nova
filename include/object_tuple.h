#ifndef OBJECT_TUPLE_H
#define OBJECT_TUPLE_H

#include "object.h"

#define IS_TUPLE(value)         isObjType(value, OBJ_TUPLE)

#define AS_TUPLE(value)         ((ObjTuple*)AS_OBJ(value))

typedef struct {
    Obj obj;
    size_t size;
    Value values[];
} ObjTuple;

ObjTuple* allocateTuple(size_t size);

int tupleWrite(ObjTuple *tuple, char *buffer, const size_t size);

int tuplePrint(ObjTuple *tuple);

#endif