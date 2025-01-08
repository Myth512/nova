#ifndef OBJECT_RANGE_ITERATOR
#define OBJECT_RANGE_ITERATOR

#include "value.h"
#include "object.h"

#define AS_RANGE_ITERATOR(value)    ((ObjRangeIter*)value.as.object)

#define RANGE_ITERATOR_METHODS (ValueMethods) { \
    .iter = RangeIter_Iter,                     \
    .next = RangeIter_Next                      \
}

typedef struct {
    Obj obj;
    long long current;
    long long end;
    long long step;
} ObjRangeIter;

ObjRangeIter *allocateRangeIter(Value value);

Value RangeIter_Iter(Value value);

Value RangeIter_Next(Value value);

#endif
