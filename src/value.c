#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "value.h"
#include "object.h"
#include "object_utils.h"
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

void insertValue(ValueVec *vec, int index, Value value) {
    if (vec->size + 1 >= vec->capacity) {
        growValueVec(vec);
    }

    for (int i = vec->size; i > index; i--) {
        vec->values[i] = vec->values[i-1];
    }
    vec->values[index] = value;
    vec->size++;
}

void reverseValueVec(ValueVec *vec) {
    int size = vec->size;
    for (int i = 0; i < size / 2; i++) {
        Value tmp = vec->values[i];
        vec->values[i] = vec->values[size - i - 1];
        vec->values[size - i - 1] = tmp;
    }
}

Value popValue(ValueVec *vec) {
    return vec->values[--vec->size];
}

bool isInt(Value value) {
    if (!IS_NUMBER(value))
        return false;
    double f = AS_NUMBER(value);
    return f == (int)f;
}

int asInt(Value value) {
    return (int)AS_NUMBER(value);
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
            return compareObjects(a, b);
        default:
            return false;
    }
}

const char* decodeValueType(Value value) {
    switch (value.type) {
        case VAL_BOOL:
            return "<type bool>";
        case VAL_NIL:
            return "<type nil>";
        case VAL_NUMBER:
            return "<type number>";
        case VAL_OBJ:
            return decodeObjType(value);
    }
    return ""; // uncreachable
}

const char *decodeValueTypeClean(Value value) {
    switch (value.type) {
        case VAL_BOOL:
            return "bool";
        case VAL_NIL:
            return "nil";
        case VAL_NUMBER:
            return "number";
        case VAL_OBJ:
            return decodeObjTypeClean(value);
    }
    return ""; // uncreachable
}

uint64_t hashNumber(double value) {
    union {
        double d;
        uint64_t i;
    } u;
    u.d = value;

    uint64_t hash = u.i;
    hash ^= (hash >> 33);
    hash *= 0xff51afd7ed558ccd;
    hash ^= (hash >> 33);
    hash *= 0xc4ceb9fe1a85ec53;
    hash ^= (hash >> 33);

    return hash;
}

uint64_t hashLong(long value) {
    uint64_t hash = (uint64_t)value;
    
    hash ^= (hash >> 33);
    hash *= 0xff51afd7ed558ccd;
    hash ^= (hash >> 33);
    hash *= 0xc4ceb9fe1a85ec53;
    hash ^= (hash >> 33);

    return hash;
}

uint64_t hashBool(bool value) {
    if (value)
        return hashLong(1);
    return hashLong(0);
}

uint64_t hashNil() {
    return hashLong(-1);
}

uint64_t hashValue(Value value) {
    switch (value.type) {
        case VAL_BOOL:
            return hashBool(AS_BOOL(value));
        case VAL_NIL:
            return hashNil();
        case VAL_NUMBER:
            return hashNumber(AS_NUMBER(value));
        case VAL_OBJ:
            return hashObject(value);
    }
}