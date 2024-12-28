#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>

#include "value.h"
#include "table.h"
#include "code.h"

struct Obj {
    bool isMarked;
    struct Obj *next;
};

Obj* allocateObject(size_t size);

#endif