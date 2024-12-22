#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "value.h"
#include "object.h"
#include "object_utils.h"
#include "object_string.h"
#include "object_class.h"
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

int writeToBuffer(char *buffer, const size_t size, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int bytesWritten;

    if (buffer == NULL)
        bytesWritten = vprintf(format, args);
    else
        bytesWritten = vsnprintf(buffer, size, format, args);
    
    va_end(args);

    return bytesWritten;
}

int valueWrite(Value value, char *buffer, const size_t size) {
    switch (value.type) {
        case VAL_BOOL:
            return writeToBuffer(buffer, size, AS_BOOL(value) ? "true" : "false");
        case VAL_NIL:
            return writeToBuffer(buffer, size, "nil");
        case VAL_NUMBER:
            return writeToBuffer(buffer, size, "%g", AS_NUMBER(value));
        case VAL_OBJ:
            return objectWrite(value, buffer, size);
    }
}

int valuePrint(Value value) {
    return valueWrite(value, NULL, 0);
}

bool fequal(double a, double b) {
    double epsilon = 1e-10;
    return fabs(a - b) < epsilon;
}

bool fnotEqual(double a, double b) {
    return !fequal(a, b);
}

bool equal(bool a, bool b) {
    return a == b;
}

bool notEqual(bool a, bool b) {
    return a != b;
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

double add(double a, double b) {
    return a + b; 
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b; 
}

double divide(double a, double b) {
    if (b == 0)
        reportRuntimeError("Division by zero");
    return a / b;
}

double modulo(double a, double b) {
    if (b == 0)
        reportRuntimeError("Division by zero");
    return fmod(a, b);
}

double increment(double a) {
    return a + 1;
}

double decrement(double a) {
    return a - 1;
}

double negate(double a) {
    return -a;
}

static bool equality(Value a, Value b, bool typeMismatchValue, bool (*boolFunc)(bool, bool), bool (*numFunc)(double, double), bool (*objFunc)(Value, Value)) {
    if (IS_INSTANCE(a) || IS_INSTANCE(b))
        return objFunc(a, b);

    if (a.type != b.type)
        return typeMismatchValue;

    switch (a.type) {
        case VAL_BOOL:
            return boolFunc(AS_BOOL(a), AS_BOOL(b));
        case VAL_NIL:
            return !typeMismatchValue;
        case VAL_NUMBER:
            return numFunc(AS_NUMBER(a), AS_NUMBER(b));
        case VAL_OBJ:
            return objFunc(a, b);
    }
}

static Value unary(Value a, char *name, double (*numFunc)(double), Value (*objFunc)(Value)) {
    switch (a.type) {
        case VAL_NUMBER:
            return NUMBER_VAL(numFunc(AS_NUMBER(a)));
        case VAL_OBJ:
            return objFunc(a);
        default:
            operatorNotImplementedUnary(name, a);
    }
}

static bool inequality(Value a, Value b, char *name, bool (*numFunc)(double, double), bool (*objFunc)(Value, Value)) {
    if (IS_INSTANCE(a) || IS_INSTANCE(b))
        return objFunc(a, b);

    if (a.type != b.type)
        operatorNotImplemented(name, a, b);

    switch (a.type) {
        case VAL_NUMBER:
            return numFunc(AS_NUMBER(a), AS_NUMBER(b));
        case VAL_OBJ:
            return objFunc(a, b);
        default:
            operatorNotImplemented(name, a, b);
    }
}

static Value arithmetic(Value a, Value b, char *name, double (*numFunc)(double, double), Value (*objFunc)(Value, Value)) {
    if (IS_INSTANCE(a) || IS_INSTANCE(b))
        return objFunc(a, b);
    
    if (IS_STRING(a) || IS_STRING(b))
        return objFunc(a, b);

    switch (a.type) {
        case VAL_NUMBER:
            return NUMBER_VAL(numFunc(AS_NUMBER(a), AS_NUMBER(b)));
        case VAL_OBJ:
            return objFunc(a, b);
        default:
            operatorNotImplemented(name, a, b);
    }
}

bool valueEqual(Value a, Value b) {
    return equality(a, b, false, equal, fequal, objectEqual);
}

bool valueNotEqual(Value a, Value b) {
    return equality(a, b, true, notEqual, fnotEqual, objectNotEqual);
}

bool valueGreater(Value a, Value b) {
    return inequality(a, b, ">", greater, objectGreater);
}

bool valueGreaterEqual(Value a, Value b) {
    return inequality(a, b, ">=", greaterEqual, objectGreaterEqual);
}

bool valueLess(Value a, Value b) {
    return inequality(a, b, "<", less, objectLess);
}

bool valueLessEqual(Value a, Value b) {
    return inequality(a, b, "<=", lessEqual, objectLessEqual);
}

Value valueAdd(Value a, Value b) {
    return arithmetic(a, b, "+", add, objectAdd);
}

Value valueSubtract(Value a, Value b) {
    return arithmetic(a, b, "-", subtract, objectSubtract);
}

Value valueMultiply(Value a, Value b) {
    return arithmetic(a, b, "*", multiply, objectMultiply);
}

Value valueDivide(Value a, Value b) {
    return arithmetic(a, b, "/", divide, objectDivide);
}

Value valueModulo(Value a, Value b) {
    return arithmetic(a, b, "%%", modulo, objectModulo);
}

Value valuePower(Value a, Value b) {
    return arithmetic(a, b, "^", pow, objectPower);
}

Value valueNegate(Value a) {
    return unary(a, "-", negate, objectNegate);
}

Value valueIncrement(Value a) {
    return unary(a, "++", increment, objectIncrement);
}

Value valueDecrement(Value a) {
    return unary(a, "--", decrement, objectDecrement);
}

OptValue valueGetField(Value obj, ObjString *name) {
    if (!IS_OBJ(obj))
        reportRuntimeError("%s does not have fields", decodeValueType(obj));
    return objectGetField(obj, name);
}

void valueSetField(Value obj, ObjString *name, Value value) {
    if (!IS_OBJ(obj))
        reportRuntimeError("%s does not have fields", decodeValueType(obj));
    objectSetField(obj, name, value);
}

Value valueGetAt(Value obj, Value key) {
    if (!IS_OBJ(obj))
        reportRuntimeError("%s is not subscriptalbe", decodeValueType(obj));
    return objectGetAt(obj, key);
}

void valueSetAt(Value obj, Value key, Value value) {
    if (!IS_OBJ(obj))
        reportRuntimeError("%s does not support item assignement", decodeValueType(obj));
    objectSetAt(obj, key, value);
}

uint64_t valueAddr(Value value) {
    return (uint64_t)&value;
}

int valueLen(Value value) {
    switch (value.type) {
        case VAL_OBJ:
            return objectLen(value);
        default:
            functionNotImplemented("len", value);
    }
}

bool valueToBool(Value value) {
    switch (value.type) {
        case VAL_BOOL:
            return AS_BOOL(value);
        case VAL_NIL:
            return false;
        case VAL_NUMBER:
            return AS_NUMBER(value) != 0;
        case VAL_OBJ:
            return objectToBool(value);
    }
}

int valueToInt(Value value) {
    switch (value.type) {
        case VAL_BOOL:
            return (int)AS_BOOL(value);
        case VAL_NIL:
            return 0;
        case VAL_NUMBER:
            return (int)AS_NUMBER(value);
        case VAL_OBJ:
            return objectToInt(value);
    }
}

double valueToFloat(Value value) {
    switch(value.type) {
        case VAL_BOOL:
            return (double)AS_BOOL(value);
        case VAL_NIL:
            return 0.0f;
        case VAL_NUMBER:
            return AS_NUMBER(value);
        case VAL_OBJ:
            return objectToFloat(value);
    }
}

ObjString *valueToStr(Value value) {
    char buffer[128];
    int length = valueWrite(value, buffer, sizeof(buffer));
    ObjString *string = copyString(buffer, length);
    return string;
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

uint64_t valueHash(Value value) {
    switch (value.type) {
        case VAL_BOOL:
            return hashBool(AS_BOOL(value));
        case VAL_NIL:
            return hashNil();
        case VAL_NUMBER:
            return hashNumber(AS_NUMBER(value));
        case VAL_OBJ:
            return objectHash(value);
    }
}