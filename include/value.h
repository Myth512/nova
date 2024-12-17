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

void printValue(Value value);

int writeValue(Value value, char *buffer, const size_t maxSize);

bool greater(double a, double b);

bool greaterEqual(double a, double b);

bool less(double a, double b);

bool lessEqual(double a, double b);

Value valueEqual(Value a, Value b);

Value valueNotEqual(Value a, Value b);

Value valueGreater(Value a, Value b);

Value valueGreaterEqual(Value a, Value b);

Value valueLess(Value a, Value b);

Value valueLessEqual(Value a, Value b);

Value valueNot(Value a);

Value valueAdd(Value a, Value b);

Value valueSubtract(Value a, Value b);

Value valueMultiply(Value a, Value b);

Value valueDivide(Value a, Value b);

Value valueModulo(Value a, Value b);

Value valuePower(Value a, Value b);

Value valueNegate(Value a);

Value valueIncrement(Value a);

Value valueDecrement(Value a);

const char* decodeValueType(Value value);

const char *decodeValueTypeClean(Value value);

uint64_t hashLong(long value);

uint64_t valueHash(Value value);

uint64_t valueAddr(Value value);

#endif