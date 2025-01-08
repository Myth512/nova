#include "object_range_iterator.h"
#include "object_range.h"
#include "value_int.h"
#include "object_exception.h"

ObjRangeIter *allocateRangeIter(Value value) {
    ObjRangeIter *iter = (ObjRangeIter*)allocateObject(sizeof(ObjRangeIter), VAL_RANGE_ITERATOR);
    ObjRange *range = AS_RANGE(value);
    iter->current = range->start;
    iter->end = range->end;
    iter->step = range->step;
    return iter;
}

Value RangeIter_Iter(Value value) {
    return value;
}

Value RangeIter_Next(Value value) {
    ObjRangeIter *iter = AS_RANGE_ITERATOR(value);
    if (iter->current >= iter->end)
        return createMsgException("", VAL_STOP_ITERATION);
    Value res = INT_VAL(iter->current);
    iter->current += iter->step;
    return res;
}