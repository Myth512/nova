#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct Obj Obj;
typedef struct ObjString ObjString;
typedef struct ObjRawString ObjRawString;

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
    VAL_OBJ
} ValueType;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
        Obj *obj;
    } as;
} Value;

typedef struct {
    bool hasValue;
    Value value;
} OptValue;

#define BOOL_VAL(value)     ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL             ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value)   ((Value){VAL_NUMBER, {.number = value}}) 
#define OBJ_VAL(object)     ((Value){VAL_OBJ, {.obj = (Obj*)object}})

#define IS_BOOL(value)      ((value).type == VAL_BOOL)
#define IS_NIL(value)       ((value).type == VAL_NIL)
#define IS_NUMBER(value)    ((value).type == VAL_NUMBER)
#define IS_OBJ(value)       ((value).type == VAL_OBJ)

#define AS_BOOL(value)      ((value).as.boolean)
#define AS_NUMBER(value)    ((value).as.number)
#define AS_OBJ(value)       ((value).as.obj)

bool isInt(Value value);

int asInt(Value value);

void valuePrint(Value value);

int writeValue(Value value, char *buffer, const size_t maxSize);

bool greater(double a, double b);

bool greaterEqual(double a, double b);

bool less(double a, double b);

bool lessEqual(double a, double b);

bool valueEqual(Value a, Value b);

bool valueNotEqual(Value a, Value b);

bool valueGreater(Value a, Value b);

bool valueGreaterEqual(Value a, Value b);

bool valueLess(Value a, Value b);

bool valueLessEqual(Value a, Value b);

Value valueAdd(Value a, Value b);

Value valueSubtract(Value a, Value b);

Value valueMultiply(Value a, Value b);

Value valueDivide(Value a, Value b);

Value valueModulo(Value a, Value b);

Value valuePower(Value a, Value b);

Value valueNegate(Value a);

Value valueIncrement(Value a);

Value valueDecrement(Value a);

uint64_t hashLong(long value);

uint64_t valueHash(Value value);

uint64_t valueAddr(Value value);

int valueLen(Value value);

bool valueToBool(Value value);

int valueToInt(Value value);

double valueToFloat(Value value);

const char* decodeValueType(Value value);

const char *decodeValueTypeClean(Value value);

#endif