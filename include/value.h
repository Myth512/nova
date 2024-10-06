#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

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

typedef struct {
    int size;
    int capacity;
    Value *values;
} ValueVec;

void initValueVec(ValueVec *values);

void freeValueVec(ValueVec *values);

void pushValue(ValueVec *values, Value value);

void printValue(Value value);

int writeValue(Value value, char *buffer, const size_t maxSize);

bool compareValues(Value a, Value b);

#endif