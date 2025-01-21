#ifndef OBJECT_RANGE_H
#define OBJECT_RANGE_H

#include "object.h"

#define IS_RANGE(value)     (value.type == VAL_RANGE)

#define AS_RANGE(value)     ((ObjRange*)value.as.object)

#define RANGE_METHODS (ValueMethods) { \
    .init = Range_Init,               \
    .contains = Range_Contains,       \
    .iter = Range_Iter,               \
    .getitem = Range_GetItem,         \
    .len = Range_Len,                 \
    .str = Range_ToStr,               \
    .repr = Range_ToStr,              \
}

typedef struct {
    Obj obj;
    long long start;
    long long end;
    long long step;
} ObjRange;

ObjRange *allocateRange(long long start, long long end, long long step);

Value Range_Init(Value callee, int argc, Value *argv);

Value Range_Contains(Value a, Value b);

Value Range_GetItem(Value value, Value key);

Value Range_Class(Value value);

Value Range_Iter(Value value);

long long Range_Len(Value value);

int Range_ToStr(Value value, char *buffer, size_t size);

#endif