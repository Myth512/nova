#include "py_range.h"
#include "object_range.h"
#include "value_int.h"
#include "vm.h"

Value PyRange_Start(Value self) {
    return INT_VAL(AS_RANGE(self)->start);
}

Value PyRange_Step(Value self) {
    return INT_VAL(AS_RANGE(self)->step);
}

Value PyRange_Stop(Value self) {
    return INT_VAL(AS_RANGE(self)->end);
}

Value PyRange_Class(Value self) {
    return TYPE_CLASS(range);
}