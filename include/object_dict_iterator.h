#ifndef OBJECT_DICT_ITERATOR
#define OBJECT_DICT_ITERATOR

#include "value.h"
#include "object.h"
#include "table.h"

#define AS_DICT_ITERATOR(value)    ((ObjDictIterator*)value.as.object)

#define DICT_ITERATOR_METHODS (ValueMethods) { \
    .iter = DictIterator_Iter,                 \
    .next = DictIterator_Next,                 \
    .class = DictIterator_Class,               \
}

typedef struct {
    Obj obj;
    Entry *current;
    Entry *end;
} ObjDictIterator;

ObjDictIterator *allocateDictIterator(Value value);

Value DictIterator_Iter(Value value);

Value DictIterator_Next(Value value);

Value DictIterator_Class(Value value);

#endif
