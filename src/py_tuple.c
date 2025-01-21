#include "py_tuple.h"
#include "object_tuple.h"
#include "value_int.h"
#include "object_string.h"
#include "object_exception.h"
#include "vm.h"

Value PyTuple_Count(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    long long count = 0;
    ObjTuple *tuple = AS_TUPLE(self);

    for (int i = 0; i < tuple->size; i++)
        count += valueToBool(valueEqual(tuple->values[i], value));
    
    return INT_VAL(count);
}

Value PyTuple_Index(int argc, int kwargc) {
    static char *keywords[] = {"self", "value", "start", "stop"};
    Value self, value, start, stop;
    PARSE_ARGS(&self, &value, &start, &stop);

    long long istart;
    long long istop;
    ObjTuple *tuple = AS_TUPLE(self);

    if (IS_UNDEFINED(start))
        istart = 0;
    else
        istart = valueToInt(start);
    
    if (IS_UNDEFINED(stop))
        istop = tuple->size;
    else
        istop = valueToInt(stop);

    for (int i = istart; i < istop; i++) {
        if (valueToBool(valueEqual(tuple->values[i], value)))
            return INT_VAL(i);
    }

    return createException(VAL_VALUE_ERROR, "%s is not in tuple", valueToStr(value)->chars);
}