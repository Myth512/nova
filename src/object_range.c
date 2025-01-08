#include "object_range.h"
#include "object_range_iterator.h"
#include "value_int.h"
#include "value_methods.h"
#include "vm.h"

ObjRange *allocateRange(long long start, long long end, long long step) {
    ObjRange *range = (ObjRange*)allocateObject(sizeof(ObjRange), VAL_RANGE);
    range->start = start;
    range->end = end;
    range->step = step;
}

Value Range_Init(Value callee, int argc, Value *argv) {
    if (argc == 1)
        return OBJ_VAL(allocateRange(0, valueToInt(argv[0]), 1));
    if (argc == 2)
        return OBJ_VAL(allocateRange(valueToInt(argv[0]), valueToInt(argv[1]), 1));
    if (argc == 3)
        return OBJ_VAL(allocateRange(valueToInt(argv[0]), valueToInt(argv[1]), valueToInt(argv[2])));
    reportArityError(1, 3, argc);
}

Value Range_Contains(Value a, Value b) {
    if (!IS_INT(b))
        return BOOL_VAL(false);

    ObjRange *range = AS_RANGE(a);
    long long number = AS_INT(b);

    if ((range->step > 0 && (number < range->start || number > range->end)) ||
        (range->step < 0 && (number > range->start || number < range->end))) {
        return BOOL_VAL(false);
    } 

    return BOOL_VAL(((number - range->start) % (range->step > 0 ? range->step : -range->step)) == 0);
}

Value Range_Iter(Value value) {
    return OBJ_VAL(allocateRangeIterator(value));
}

Value Range_GetItem(Value value, Value key) {
    if (!IS_INT(key))
        reportRuntimeError("must be int");
    ObjRange *range = AS_RANGE(value);
    long long len = Range_Len(value);
    long long index = calculateIndex(AS_INT(key), len);
    return INT_VAL(range->start + range->step * index);
}

Value Range_Class(Value value) {
    return TYPE_CLASS(range);
}

long long Range_Len(Value value) {
    ObjRange *range = AS_RANGE(value);
    return (range->end - range->start + range->step - (range->step > 0 ? 1 : -1)) / range->step;
}

int Range_ToStr(Value value, char *buffer, size_t size) {
    ObjRange *range = AS_RANGE(value);
    if (range->step != 1)
        return writeToBuffer(buffer, size, "range(%lld, %lld, %lld)", range->start, range->end, range->step);
    return writeToBuffer(buffer, size, "range(%lld, %lld)", range->start, range->end);
}