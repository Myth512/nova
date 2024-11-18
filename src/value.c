#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "value.h"
#include "object.h"
#include "memory.h"

void initValueVec(ValueVec *vec) {
    vec->size = 0;
    vec->capacity = 0;
    vec->values = NULL;
}

void freeValueVec(ValueVec *vec) {
    FREE_VEC(Value, vec->values, vec->capacity);
    initValueVec(vec);
}

void growValueVec(ValueVec *vec) {
    int oldCapacity = vec->capacity;
    vec->capacity = GROW_CAPACITY(oldCapacity);
    vec->values = GROW_VEC(Value, vec->values, oldCapacity, vec->capacity);
}

void pushValue(ValueVec *vec, Value value) {
    if (vec->size + 1 >= vec->capacity) {
        growValueVec(vec);
    }

    vec->values[vec->size++] = value;
}

void printValue(Value value) {
    switch (value.type) {
        case VAL_BOOL:
            printf(AS_BOOL(value) ? "true" : "false");
            break;
        case VAL_NIL:
            printf("nil");
            break;
        case VAL_NUMBER:
            printf("%g", AS_NUMBER(value));
            break;
        case VAL_OBJ:
            printObject(value);
            break;
    }
}

int writeValue(Value value, char *buffer, const size_t maxSize) {
    switch (value.type) {
        case VAL_BOOL:
            if (AS_BOOL(value))
                return snprintf(buffer, 5, "true");
            return snprintf(buffer, 6, "false");
        case VAL_NIL:
            return snprintf(buffer, 4, "nil");
        case VAL_NUMBER:
            return snprintf(buffer, maxSize, "%g", AS_NUMBER(value));
        case VAL_OBJ:
            return writeObject(value, buffer, maxSize);
    }
    return -1; // unreachable
}

bool compareValues(Value a, Value b) {
    if (a.type != b.type)
        return false;

    switch (a.type) {
        case VAL_BOOL:
            return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL:
            return true;
        case VAL_NUMBER:
            return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_OBJ:
            ObjString *aString = AS_STRING(a);
            ObjString *bString = AS_STRING(b);
            return aString->length == bString->length
                && memcmp(aString->chars, bString->chars, aString->length) == 0;
        default:
            return false;
    }
}

const char* decodeValueType(Value value) {
    switch (value.type) {
        case VAL_BOOL:
            return "BOOL";
        case VAL_NIL:
            return "NIL";
        case VAL_NUMBER:
            return "NUMBER";
        case VAL_OBJ:
            return decodeObjType(value);
    }
    return ""; // uncreachable
}
