#ifndef OBJECT_SUPER_H
#define OBJECT_SUPER_H

#include "value.h"
#include "object.h"

#define IS_SUPER(value)     (value.type == VAL_SUPER)

#define AS_SUPER(value)     ((ObjSuper*)value.as.object)

#define SUPER_METHODS (ValueMethods) { \ 
    .init = Super_Init,                \
    .getattr = Super_GetAttr,          \
    .str = Super_ToStr,                \
    .repr = Super_ToStr,               \
}

typedef struct {
    Obj obj;
    Value self;
    Value class;
} ObjSuper;

ObjSuper *allocateSuper(Value self, Value class);

Value Super_Init(Value callee, int argc, Value *argv);

Value Super_GetAttr(Value value, ObjString *name);

int Super_ToStr(Value value, char *buffer, const size_t size);

#endif