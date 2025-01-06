#include <stdio.h>

#include "value_methods.h"
#include "object_tuple.h"

ObjTuple* allocateTuple(size_t size) {
    ObjTuple *tuple = (ObjTuple*)allocateObject(sizeof(ObjTuple) + size * sizeof(Value), VAL_TUPLE);
    tuple->size = size;
    return tuple;
}

int Tuple_Index(Value obj, Value value, int start, int end) {
    size_t size = AS_TUPLE(obj)->size;
    if (end == 0)
        end = size;

    for (int i = start; i < end; i++) {
        if (valueToBool(valueEqual(AS_TUPLE(obj)->values[i], value)))
            return i;
    }
    return -1;
}

int Tuple_ToStr(Value value, char *buffer, size_t size) {
    size_t bytesLeft = size;
    int bytesWritten = writeToBuffer(buffer, bytesLeft, "(");
    bytesLeft -= movePointer(&buffer, bytesWritten);
    size_t length = AS_TUPLE(value)->size; 
    for (int i = 0; i < length; i++) {
        bytesWritten = valueWrite(AS_TUPLE(value)->values[i], buffer, bytesLeft);
        bytesLeft -= movePointer(&buffer, bytesWritten);
        if (i + 1 != length) {
            bytesWritten = writeToBuffer(buffer, bytesLeft, ", ");
            bytesLeft -= movePointer(&buffer, bytesWritten);
        }
    }
    writeToBuffer(buffer, size, ")");
}