#include "object_list_iterator.h"
#include "object_list.h"
#include "object.h"
#include "object_exception.h"
#include "vm.h"

ObjListIterator *allocateListIterator(Value value) {
    ObjListIterator *iter = (ObjListIterator*)allocateObject(sizeof(ObjListIterator), VAL_LIST_ITERATOR);
    ObjList *list = AS_LIST(value);
    iter->current = list->vec.values;
    iter->end = list->vec.values + list->vec.size;
    return iter;
}

Value ListIterator_Iter(Value value) {
    return value;
}

Value ListIterator_Next(Value value) {
    ObjListIterator *iter = AS_LIST_ITERATOR(value);
    if (iter->current >= iter->end)
        return createException(VAL_STOP_ITERATION, "");
    Value res = *iter->current;
    iter->current++;
    return res;
}

Value ListIterator_Class(Value value) {
    return TYPE_CLASS(listIterator);
}