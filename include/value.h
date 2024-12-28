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
    VAL_NOT_IMPLEMENTED,
    VAL_STRING,
    VAL_LIST,
    VAL_TUPLE,
    VAL_FUNCTION,
    VAL_CLOSURE,
    VAL_UPVALUE,
    VAL_NATIVE,
    VAL_CLASS,
    VAL_METHOD,
    VAL_NATIVE_METHOD,
    VAL_INSTANCE,
} ValueType;

typedef struct {
    ValueType type;
    union {
        double floating;
        long long integer;
        Obj *object;
    } as;
} Value;

typedef struct {
    bool hasValue;
    Value value;
} OptValue;

#define NONE_VAL             ((Value){VAL_NONE})
#define UNDEFINED_VAL       ((Value){VAL_UNDEFINED})
#define NOT_IMPLEMENTED_VAL ((Value){VAL_NOT_IMPLEMENTED})

#define IS_NONE(value)      ((value).type == VAL_NONE)
#define IS_NOT_IMPLEMENTED(value)  ((value).type == VAL_NOT_IMPLEMENTED)
#define IS_UNDEFINED(value) ((value).type == VAL_UNDEFINED)

const char* decodeValueType(Value value);

const char *decodeValueTypeClean(Value value);

#endif