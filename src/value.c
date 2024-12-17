#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "value.h"
#include "object.h"
#include "object_utils.h"
#include "memory.h"
#include "error.h"
#include "vm.h"

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

bool fequal(double a, double b) {
    double epsilon = 1e-10;
    return fabs(a - b) < epsilon;
}

bool greater(double a, double b) {
    return a > b;
}

bool greaterEqual(double a, double b) {
    return a >= b;
}

bool less(double a, double b) {
    return a < b;
}

bool lessEqual(double a, double b) {
    return a <= b;
}

bool valueEqual(Value a, Value b) {
    if (a.type != b.type)
        return false;

    switch (a.type) {
        case VAL_BOOL:
            return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL:
            return true;
        case VAL_NUMBER:
            return fequal(AS_NUMBER(a), AS_NUMBER(b));
        case VAL_OBJ:
            return objectEqual(a, b);
    }
}

bool valueNotEqual(Value a, Value b) {
    return !valueEqual(a, b);
}

static bool equality(Value a, Value b, char *name, bool (*numFunc)(double, double), bool (*objFunc)(Value, Value)) {
    if (a.type != b.type)
        reportTypeError(name, a, b);

    switch (OBJ_TYPE(a)) {
        case VAL_NUMBER:
            return numFunc(AS_NUMBER(a), AS_NUMBER(b));
        case VAL_OBJ:
            return objFunc(a, b);
        default:
            reportTypeError(name, a, b);
    }
}

bool valueGreater(Value a, Value b) {
    return equality(a, b, ">", greater, valueGreater);
}

bool valueGreaterEqual(Value a, Value b) {
    return equality(a, b, ">=", greaterEqual, valueGreaterEqual);
}

bool valueLess(Value a, Value b) {
    return equality(a, b, "<", less, valueLess);
}

bool valueLessEqual(Value a, Value b) {
    return equality(a, b, "<=", lessEqual, valueLessEqual);
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