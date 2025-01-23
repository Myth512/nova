#include "object_range_iterator.h"
#include "object_range.h"
#include "value_int.h"
#include "object_exception.h"
#include "vm.h"

ObjRangeIterator *allocateRangeIterator(Value value) {
    ObjRangeIterator *iter = (ObjRangeIterator*)allocateObject(sizeof(ObjRangeIterator), VAL_RANGE_ITERATOR);
    ObjRange *range = AS_RANGE(value);
    iter->current = range->start;
    iter->end = range->end;
    iter->step = range->step;
    return iter;
}

Value RangeIterator_Iter(Value value) {
    return value;
}

Value RangeIterator_Next(Value value) {
    ObjRangeIterator *iter = AS_RANGE_ITERATOR(value);
    if (iter->step > 0) {
        if (iter->current >= iter->end)
            return createException(VAL_STOP_ITERATION, "");
    } else {
        if (iter->current <= iter->end)
            return createException(VAL_STOP_ITERATION, "");
    }
    Value res = INT_VAL(iter->current);
    iter->current += iter->step;
    return res;
}

Value RangeIterator_Class(Value value) {
    return TYPE_CLASS(rangeIterator);
}