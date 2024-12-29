#include <stdio.h>

#include "object_tuple.h"

ObjTuple* allocateTuple(size_t size) {
    ObjTuple *tuple = (ObjTuple*)allocateObject(sizeof(ObjTuple) + size * sizeof(Value));
    tuple->size = size;
    return tuple;
}

static int movePointer(char **buffer, int bytesWritten) {
    if (*buffer != NULL) {
        *buffer += bytesWritten;
        return bytesWritten;
    }
    return 0;
}

int tupleWrite(ObjTuple *tuple, char *buffer, const size_t size) {
    size_t bytesLeft = size;
    int bytesWritten = writeToBuffer(buffer, bytesLeft, "(");
    bytesLeft -= movePointer(&buffer, bytesWritten);
    size_t length = tuple->size; 
    for (int i = 0; i < length; i++) {
        bytesWritten = valueWrite(tuple->values[i], buffer, bytesLeft);
        bytesLeft -= movePointer(&buffer, bytesWritten);
        if (i + 1 != length) {
            bytesWritten = writeToBuffer(buffer, bytesLeft, ", ");
            bytesLeft -= movePointer(&buffer, bytesWritten);
        }
    }
    writeToBuffer(buffer, size, ")");
}

int tuplePrint(ObjTuple *tuple) {
    return tupleWrite(tuple, NULL, 0);
}