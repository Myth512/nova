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

// bool compareArrays(ObjArray *a, ObjArray *b) {
//     if (a->vec.size != b->vec.size)
//         return false;
//     for (int i = 0; i < a->vec.size; i++) {
//         if (!compareValues(a->vec.values[i], b->vec.values[i]))
//             return false;
//     }
//     return true;
// }

// void printArray(ObjArray *array) {
//     printf("[");
//     size_t size = array->vec.size;
//     for (int i = 0; i < size; i++) {
//         printValue(array->vec.values[i]);
//         if (i + 1 != size) {
//             printf(", ");
//         }
//     }
//     printf("]");
// }


// static void repeatArray() {
//     ObjArray *array;
//     double number;

//     if (IS_ARRAY(peek(0))) {
//         array = AS_ARRAY(pop());
//         number = AS_NUMBER(pop());
//     } else {
//         number = AS_NUMBER(pop());
//         array = AS_ARRAY(pop());
//     }

//     if (number != (int)number)
//         reportRuntimeError("Can't multiply array by non whole number");

//     size_t initSize = array->vec.size;
//     size_t newSize = initSize * number;

//     ObjArray *result = allocateArray(newSize);

//     for (int i = 0; i < newSize; i++) {
//         result->vec.values[i] = array->vec.values[i % initSize];
//     }

//     push(OBJ_VAL(result));
// }

// static void concatenateArrays() {
//     ObjArray *a = AS_ARRAY(pop());
//     ObjArray *b = AS_ARRAY(pop());

//     size_t aSize = a->vec.size;
//     size_t bSize = b->vec.size;

//     size_t size = aSize + bSize; 

//     ObjArray *result = allocateArray(size);

//     for (int i = 0; i < aSize; i++) {
//         result->vec.values[i] = a->vec.values[i];
//     }
//     for (int i = 0; i < bSize; i++) {
//         result->vec.values[aSize + i] = b->vec.values[i];
//     }

//     push(OBJ_VAL(result));
// }

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