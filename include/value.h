#ifndef VALUE_H
#define VALUE_H

#include <stdlib.h>
#include <stdbool.h>

#include "memory.h"

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
} ValueType;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
    } as;
} Value;

#define BOOL_VAL(value)     ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL             ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value)   ((Value){VAL_NUMBER, {.number = value}}) 

#define IS_BOOL(value)      ((value).type == VAL_BOOL)
#define IS_NIL(value)       ((value).type == VAL_NIL)
#define IS_NUMBER(value)    ((value).type == VAL_NUMBER)

#define AS_BOOL(value)      ((value).as.boolean)
#define AS_NUMBER(value)    ((value).as.number)

typedef struct {
    int size;
    int capacity;
    Value *values;
} ValueVec;

void initValueVec(ValueVec *values);

void freeValueVec(ValueVec *values);

void pushValue(ValueVec *values, Value value);

void printValue(Value value);

#endif