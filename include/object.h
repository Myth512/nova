#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>

#include "value.h"
#include "table.h"
#include "code.h"

struct Obj {
    ValueType type;
    bool isMarked;
    struct Obj *next;
};

#define OBJ_VAL(value)      ((Value){.type=((Obj*)value)->type, .as.object=(Obj*)value})

Obj* allocateObject(size_t size, ValueType);

bool isObject(Value value);

#endif