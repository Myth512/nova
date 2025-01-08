#ifndef OBJECT_TUPLE_ITERATOR
#define OBJECT_TUPLE_ITERATOR

#include "value.h"
#include "object.h"

#define AS_TUPLE_ITERATOR(value)    ((ObjTupleIterator*)value.as.object)

#define TUPLE_ITERATOR_METHODS (ValueMethods) { \
    .iter = TupleIterator_Iter,                 \
    .next = TupleIterator_Next,                 \
    .class = TupleIterator_Class,               \
}

typedef struct {
    Obj obj;
    Value *current;
    Value *end;
} ObjTupleIterator;

ObjTupleIterator *allocateTupleIterator(Value value);

Value TupleIterator_Iter(Value value);

Value TupleIterator_Next(Value value);

Value TupleIterator_Class(Value value);

#endif
