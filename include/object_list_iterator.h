#ifndef OBJECT_LIST_ITERATOR
#define OBJECT_LIST_ITERATOR

#include "value.h"
#include "object.h"

#define AS_LIST_ITERATOR(value)    ((ObjListIterator*)value.as.object)

#define LIST_ITERATOR_METHODS (ValueMethods) { \
    .iter = ListIterator_Iter,                 \
    .next = ListIterator_Next,                 \
    .class = ListIterator_Class,               \
}

typedef struct {
    Obj obj;
    Value *current;
    Value *end;
} ObjListIterator;

ObjListIterator *allocateListIterator(Value value);

Value ListIterator_Iter(Value value);

Value ListIterator_Next(Value value);

Value ListIterator_Class(Value value);

#endif