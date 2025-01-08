#ifndef OBJECT_RANGE_ITERATOR
#define OBJECT_RANGE_ITERATOR

#include "value.h"
#include "object.h"

#define AS_RANGE_ITERATOR(value)    ((ObjRangeIterator*)value.as.object)

#define RANGE_ITERATOR_METHODS (ValueMethods) { \
    .iter = RangeIterator_Iter,                 \
    .next = RangeIterator_Next,                 \
    .class = RangeIterator_Class,               \
}

typedef struct {
    Obj obj;
    long long current;
    long long end;
    long long step;
} ObjRangeIterator;

ObjRangeIterator *allocateRangeIterator(Value value);

Value RangeIterator_Iter(Value value);

Value RangeIterator_Next(Value value);

Value RangeIterator_Class(Value value);

#endif
