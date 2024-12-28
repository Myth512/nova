#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef enum {
    VAL_UNDEFINED,
    VAL_NONE,
    VAL_BOOL,
    VAL_INT,
    VAL_FLOAT,
    VAL_OBJ
} ValueType;

typedef struct {
    ValueType type;
    union {
        double floating;
        long long integer;
        Obj *obj;
    } as;
} Value;

typedef struct {
    bool hasValue;
    Value value;
} OptValue;

#define BOOL_VAL(value)     ((Value){VAL_BOOL, {.integer=value}})
#define NONE_VAL             ((Value){VAL_NONE, {.integer=0}})
#define UNDEFINED_VAL       ((Value){VAL_UNDEFINED, {.integer=0}})
#define OBJ_VAL(object)     ((Value){VAL_OBJ, {.obj=(Obj*)object}})

#define IS_BOOL(value)      ((value).type == VAL_BOOL)
#define IS_NONE(value)      ((value).type == VAL_NONE)
#define IS_UNDEFINED(value) ((value).type == VAL_UNDEFINED)
#define IS_OBJ(value)       ((value).type == VAL_OBJ)

#define AS_BOOL(value)      ((value).as.integer)
#define AS_OBJ(value)       ((value).as.obj)


const char* decodeValueType(Value value);

const char *decodeValueTypeClean(Value value);

#endif