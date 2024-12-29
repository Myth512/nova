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

ObjArray* allocateArray(int size);

int arrayWrite(ObjArray *array, char *buffer, const size_t size);

int arrayPrint(ObjArray *array);

bool arrayEqual(ObjArray *a, ObjArray *b);

bool arrayNotEqual(ObjArray *a, ObjArray *b);

bool arrayGreater(ObjArray *a, ObjArray *b);

bool arrayGreaterEqual(ObjArray *a, ObjArray *b);

bool arrayLess(ObjArray *a, ObjArray *b);

bool arrayLessEqual(ObjArray *a, ObjArray *b);

ObjArray *arrayAdd(ObjArray *a, ObjArray *b);
 
ObjArray *arrayMultiply(ObjArray *array, int scalar);

OptValue arrayGetField(Value array, ObjString *name);

Value arrayGetAt(ObjArray *array, Value index);

void arraySetAt(ObjArray *array, Value index, Value value);

int arrayLen(ObjArray *array);

bool arrayToBool(ObjArray *array);

#endif