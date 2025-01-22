#include "object_slice.h"
#include "value_methods.h"
#include "object_string.h"
#include "value_int.h"
#include "vm.h"

ObjSlice *allocateSlice(Value start, Value stop, Value step) {
    ObjSlice *slice = (ObjSlice*)allocateObject(sizeof(ObjSlice), VAL_SLICE);
    slice->start = start;
    slice->step = step;
    slice->stop = stop;
    return slice;
}

ParsedSlice parseSlice(ObjSlice *slice, long long length) {
    ParsedSlice result;

    if (IS_NONE(slice->start))
        result.start = 0;
    else {
        int index = calculateIndex(valueToInt(slice->start), length);
        if (index == -1)
            result.start = 0;
        else if (index == -2)
            result.start = length;
        else
            result.start = index;
    }

    if (IS_NONE(slice->stop))
        result.stop = length;
    else {
        int index = calculateIndex(valueToInt(slice->stop), length);
        if (index == -1)
            result.stop = 0;
        else if (index == -2)
            result.stop = length;
        else
            result.stop = index;
    }

    if (IS_NONE(slice->step))
        result.step = 1;
    else
        result.step = valueToInt(slice->step);
    
    result.length = (result.stop - result.start + result.step - 1) / result.step;
    
    return result;
}

Value Slice_Equal(Value a, Value b) {
    ObjSlice *s1 = AS_SLICE(a);
    ObjSlice *s2 = AS_SLICE(b);
    return BOOL_VAL(valueToBool(valueEqual(s1->start, s2->start)) && valueToBool(valueEqual(s1->stop, s2->stop)) && valueToBool(valueEqual(s1->step, s2->step)));
}

Value Slice_NotEqual(Value a, Value b) {
    return BOOL_VAL(!AS_BOOL(Slice_Equal(a, b)));
}

Value Slice_Init(Value callee, int argc, Value *argv) {
    if (argc == 1)
        return OBJ_VAL(allocateSlice(NONE_VAL, argv[0], NONE_VAL));
    if (argc == 2)
        return OBJ_VAL(allocateSlice(argv[0], argv[1], NONE_VAL));
    if (argc == 3)
        return OBJ_VAL(allocateSlice(argv[0], argv[1], argv[2]));
    reportArityError(1, 3, argc);
}

Value Slice_Class(Value self) {
    return TYPE_CLASS(slice);
}

int Slice_ToStr(Value value, char *buffer, size_t size) {
    return writeToBuffer(buffer, size, "slice(%s, %s, %s)", valueToStr(AS_SLICE(value)->start)->chars, valueToStr(AS_SLICE(value)->stop)->chars, valueToStr(AS_SLICE(value)->step)->chars);
}