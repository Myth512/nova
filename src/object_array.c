#include <stdio.h>

#include "object_array.h"
#include "object_string.h"
#include "object_class.h"
#include "array_methods.h"
#include "memory.h"
#include "vm.h"

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

static int movePointer(char **buffer, int bytesWritten) {
    if (*buffer != NULL) {
        *buffer += bytesWritten;
        return bytesWritten;
    }
    return 0;
}

int arrayWrite(ObjArray *array, char *buffer, size_t size) {
    int bytesWritten = writeToBuffer(buffer, size, "[");
    size -= movePointer(&buffer, bytesWritten);
    size_t length = array->vec.size;
    for (int i = 0; i < length; i++) {
        bytesWritten = valueWrite(array->vec.values[i], buffer, size);
        size -= movePointer(&buffer, bytesWritten);
        if (i + 1 != length) {
            bytesWritten = writeToBuffer(buffer, size, ", ");
            size -= movePointer(&buffer, bytesWritten);
        }
    }
    writeToBuffer(buffer, size, "]");
}

int arrayPrint(ObjArray *array) {
    return arrayWrite(array, NULL, 0);
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

OptValue arrayGetField(Value array, ObjString *name) {
    const struct ArrayMethod *result = in_array_set(name->chars, name->length);
    if (result) {
        ObjNativeMethod *method = createNativeMethod(array, result->method, result->name);
        return (OptValue){.hasValue=true, .value=OBJ_VAL(method)};
    } 
    return (OptValue){.hasValue=false};
}

Value arrayGetAt(ObjArray *array, Value index) {
    if (!isInt(index))
        reportRuntimeError("Index must be integer number");
    
    int i = asInt(index);
    
    int length = array->vec.size; 
    if (i >= length || i < -length)
        reportRuntimeError("Index is out of range");
    if (i < 0)
        i += length;
    
    return array->vec.values[i];
}

void arraySetAt(ObjArray *array, Value index, Value value) {
    if (!isInt(index))
        reportRuntimeError("Index must be integer number");
    
    int i = asInt(index);
    
    int length = array->vec.size; 
    if (i >= length || i < -length)
        reportRuntimeError("Index is out of range");
    if (i < 0)
        i += length;
    
    array->vec.values[i] = value;
}

int arrayLen(ObjArray *array) {
    return array->vec.size;
}

bool arrayToBool(ObjArray *array) {
    return (bool)arrayLen(array);
}