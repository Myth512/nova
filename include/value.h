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

bool valueEqual(Value a, Value b);

bool valueNotEqual(Value a, Value b);

bool valueGreater(Value a, Value b);

bool valueGreaterEqual(Value a, Value b);

bool valueLess(Value a, Value b);

bool valueLessEqual(Value a, Value b);

const char* decodeValueType(Value value);

const char *decodeValueTypeClean(Value value);

uint64_t hashLong(long value);

uint64_t hashValue(Value value);

#endif