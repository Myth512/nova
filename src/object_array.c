#include <stdio.h>

#include "object_array.h"
#include "memory.h"

ObjArray* allocateArray(int size) {
    ObjArray *array = (ObjArray*)allocateObject(sizeof(ObjArray), OBJ_ARRAY);
    array->values.size = size; 
    array->values.capacity = size; 
    array->values.values = (Value*)reallocate(array->values.values, 0, size * sizeof(Value));
    return array;
}

bool compareArrays(ObjArray *a, ObjArray *b) {
    if (a->values.size != b->values.size)
        return false;
    for (int i = 0; i < a->values.size; i++) {
        if (!compareValues(a->values.values[i], b->values.values[i]))
            return false;
    }
    return true;
}

void printArray(ObjArray *array) {
    printf("[");
    size_t size = array->values.size;
    for (int i = 0; i < size; i++) {
        printValue(array->values.values[i]);
        if (i + 1 != size) {
            printf(", ");
        }
    }
    printf("]");
}