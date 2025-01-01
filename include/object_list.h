#ifndef OBJECT_ARRAY_H
#define OBJECT_ARRAY_H

#include "object.h"
#include "value_vector.h"

#define IS_ARRAY(value)         isObjType(value, OBJ_ARRAY)

#define AS_ARRAY(value)         ((ObjArray*)value.as.object)

typedef struct {
    Obj obj;
    ValueVec vec;
} ObjArray;

#define LIST_METHODS (ValueMethods) { \
    .str = List_ToStr,                \
    .repr = List_ToStr                \
}

ObjArray* allocateArray(int size);

int List_ToStr(Value value, char *buffer, size_t size);

#endif