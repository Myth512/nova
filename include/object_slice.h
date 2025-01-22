#ifndef OBJECT_SLICE_H
#define OBJECT_SLICE_H

#include "object.h"

#define IS_SLICE(value) (value.type == VAL_SLICE)

#define AS_SLICE(value) ((ObjSlice*)value.as.object)

typedef struct {
    Obj obj;
    Value start;
    Value stop;
    Value step;
} ObjSlice;

typedef struct {
    long long start;
    long long stop;
    long long step;
    long long length;
} ParsedSlice;

#define SLICE_METHODS (ValueMethods) { \
    .eq = Slice_Equal,                 \
    .ne = Slice_NotEqual,              \
    .init = Slice_Init,                \
    .class = Slice_Class,              \
    .str = Slice_ToStr,                \
    .repr = Slice_ToStr,               \
}

ObjSlice *allocateSlice(Value start, Value stop, Value step);

ParsedSlice parseSlice(ObjSlice *slice, long long length);

Value Slice_Equal(Value a, Value b);

Value Slice_NotEqual(Value a, Value b);

Value Slice_Init(Value callee, int argc, Value *argv);

Value Slice_Class(Value self);

int Slice_ToStr(Value value, char *buffer, size_t size);

#endif