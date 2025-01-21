#ifndef OBJECT_STRING_ITERATOR
#define OBJECT_STRING_ITERATOR

#include "value.h"
#include "object.h"

#define AS_STRING_ITERATOR(value)    ((ObjStringIterator*)value.as.object)

#define STRING_ITERATOR_METHODS (ValueMethods) { \
    .iter = StringIterator_Iter,                 \
    .next = StringIterator_Next,                 \
    .class = StringIterator_Class,               \
}

typedef struct {
    Obj obj;
    char *current;
} ObjStringIterator;

ObjStringIterator *allocateStringIterator(Value value);

Value StringIterator_Iter(Value value);

Value StringIterator_Next(Value value);

Value StringIterator_Class(Value value);

#endif