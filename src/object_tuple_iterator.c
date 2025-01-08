#include "object_tuple_iterator.h"
#include "object_tuple.h"
#include "object_exception.h"
#include "vm.h"

ObjTupleIterator *allocateTupleIterator(Value value) {
    ObjTupleIterator *iter = (ObjTupleIterator*)allocateObject(sizeof(ObjTupleIterator), VAL_TUPLE_ITERATOR);
    ObjTuple *tuple = AS_TUPLE(value);
    iter->current = tuple->values;
    iter->end = tuple->values + tuple->size;
}

Value TupleIterator_Iter(Value value) {
    return value;
}

Value TupleIterator_Next(Value value) {
    ObjTupleIterator *iter = AS_TUPLE_ITERATOR(value);
    if (iter->current >= iter->end)
        return createMsgException("", VAL_STOP_ITERATION);
    Value res = *iter->current;
    iter->current++;
    return res;
}

Value TupleIterator_Class(Value value) {
    return TYPE_CLASS(tupleIterator);
}
