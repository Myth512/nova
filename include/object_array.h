#ifndef OBJECT_ARRAY_H
#define OBJECT_ARRAY_H

#include "object.h"

#define IS_ARRAY(value)         isObjType(value, OBJ_ARRAY)

#define AS_ARRAY(value)         ((ObjArray*)AS_OBJ(value))

typedef struct {
    Obj obj;
    ValueVec values;
} ObjArray;

ObjArray* allocateArray(int size);

void printArray(ObjArray *array);

#endif