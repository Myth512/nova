#include <stdio.h>

#include "object_array.h"
#include "memory.h"

static int min(int a, int b) {
    return a < b ? a : b;
}

ObjArray* allocateArray(int size) {
    ObjArray *array = (ObjArray*)allocateObject(sizeof(ObjArray), OBJ_ARRAY);
    array->vec.size = size; 
    array->vec.capacity = size; 
    array->vec.values = (Value*)reallocate(array->vec.values, 0, size * sizeof(Value));
    return array;
}

bool arrayEqual(ObjArray *a, ObjArray *b) {
    if (a->vec.size != b->vec.size)
        return false;
    for (int i = 0; i < a->vec.size; i++) {
        if (!valueEqual(a->vec.values[i], b->vec.values[i]))
            return false;
    }
    return true;
}

bool arrayNotEqual(ObjArray *a, ObjArray *b) {
    return !arrayEqual(a, b);
}

static bool inequality(ObjArray *a, ObjArray *b, bool (*numFunc)(double, double), bool (*valFunc)(Value, Value)) {
    int minLength = min(a->vec.size, b->vec.size);
    for (int i = 0; i < minLength; i++) {
        if (valueEqual(a->vec.values[i], b->vec.values[i]))
            continue;
        return valFunc(a->vec.values[i], b->vec.values[i]);
    }
    return numFunc(a->vec.size, b->vec.size);
}

bool arrayGreater(ObjArray *a, ObjArray *b) {
    return inequality(a, b, greater, valueGreater);
}

bool arrayGreaterEqual(ObjArray *a, ObjArray *b) {
    return inequality(a, b, greaterEqual, valueGreaterEqual);
}

bool arrayLess(ObjArray *a, ObjArray *b) {
    return inequality(a, b, less, valueLess);
}

bool arrayLessEqual(ObjArray *a, ObjArray *b) {
    return inequality(a, b, lessEqual, valueLessEqual);
}

ObjArray *arrayAdd(ObjArray *a, ObjArray *b) {
    size_t size = a->vec.size + b->vec.size; 

    ObjArray *result = allocateArray(size);

    for (int i = 0; i < a->vec.size; i++)
        result->vec.values[i] = a->vec.values[i];

    for (int i = 0; i < b->vec.size; i++)
        result->vec.values[a->vec.size + i] = b->vec.values[i];

    return result;
}

ObjArray *arrayMultiply(ObjArray *array, int scalar) {
    size_t oldSize = array->vec.size;
    size_t newSize = oldSize * scalar;

    ObjArray *result = allocateArray(newSize);

    for (int i = 0; i < newSize; i++)
        result->vec.values[i] = array->vec.values[i % oldSize];

    return result;
}

int arrayLen(ObjArray *array) {
    return array->vec.size;
}

bool arrayToBool(ObjArray *array) {
    return (bool)arrayLen(array);
}

void arrayPrint(ObjArray *array) {
    printf("[");
    size_t size = array->vec.size;
    for (int i = 0; i < size; i++) {
        valuePrint(array->vec.values[i]);
        if (i + 1 != size) {
            printf(", ");
        }
    }
    printf("]");
}