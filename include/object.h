#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>

#include "value.h"
#include "table.h"
#include "code.h"

#define OBJ_TYPE(value)         (AS_OBJ(value)->type)

typedef enum {
    OBJ_CLASS,
    OBJ_METHOD,
    OBJ_NATIVE_METHOD,
    OBJ_INSTANCE,
    OBJ_CLOSURE,
    OBJ_FUNCTION,
    OBJ_NATIVE,
    OBJ_STRING,
    OBJ_UPVALUE,
    OBJ_ARRAY,
    OBJ_TUPLE,
} ObjType;

struct Obj {
    ObjType type;
    bool isMarked;
    struct Obj *next;
};

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

Obj* allocateObject(size_t size, ObjType type);

#endif