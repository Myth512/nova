#include "object_dict_iterator.h"
#include "object_dict.h"
#include "object.h"
#include "object_exception.h"
#include "vm.h"

ObjDictIterator *allocateDictIterator(Value value) {
    ObjDictIterator *iter = (ObjDictIterator*)allocateObject(sizeof(ObjDictIterator), VAL_DICT_ITERATOR);
    ObjDict *dict = AS_DICT(value);
    iter->current = dict->table.order;
    iter->end = dict->table.order + dict->table.size;
    return iter;
}

Value DictIterator_Iter(Value value) {
    return value;
}

Value DictIterator_Next(Value value) {
    ObjDictIterator *iter = AS_DICT_ITERATOR(value);
    while (iter->current < iter->end) {
        Entry *entry = *iter->current; 
        iter->current++;
        return entry->key;
    }
    return createException(VAL_STOP_ITERATION, "");
}

Value DictIterator_Class(Value value) {
    return TYPE_CLASS(dictIterator);
}
