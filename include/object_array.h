#ifndef OBJECT_ARRAY_H
#define OBJECT_ARRAY_H

#include "object.h"
#include "value_vector.h"

#define IS_ARRAY(value)         isObjType(value, OBJ_ARRAY)

#define AS_ARRAY(value)         ((ObjArray*)AS_OBJ(value))

typedef struct {
    Obj obj;
    ValueVec vec;
} ObjArray;

ObjArray* allocateArray(int size);

bool arrayEqual(ObjArray *a, ObjArray *b);

bool arrayNotEqual(ObjArray *a, ObjArray *b);

bool arrayGreater(ObjArray *a, ObjArray *b);

bool arrayGreaterEqual(ObjArray *a, ObjArray *b);

bool arrayLess(ObjArray *a, ObjArray *b);

bool arrayLessEqual(ObjArray *a, ObjArray *b);

ObjArray *arrayAdd(ObjArray *a, ObjArray *b);
 
ObjArray *arrayMultiply(ObjArray *array, int scalar);

void arrayPrint(ObjArray *array);

#endif