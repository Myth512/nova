#include "py_slice.h"
#include "object_slice.h"
#include "value_methods.h"
#include "value_int.h"
#include "object_tuple.h"
#include "vm.h"

Value PySlice_Indices(int argc, int kwargc) {
    static char *keywords[] = {"self", "len"};
    Value self, len;
    PARSE_ARGS(&self, &len);

    ParsedSlice slice = parseSlice(AS_SLICE(self), valueToInt(len));

    ObjTuple *tuple = allocateTuple(3);

    tuple->values[0] = INT_VAL(slice.start);
    tuple->values[1] = INT_VAL(slice.stop);
    tuple->values[2] = INT_VAL(slice.step);

    return OBJ_VAL(tuple);
}

Value PySlice_Start(Value self) {
    return AS_SLICE(self)->start;
}

Value PySlice_Step(Value self) {
    return AS_SLICE(self)->step;
}

Value PySlice_Stop(Value self) {
    return AS_SLICE(self)->stop;
}

Value PySlice_Class(Value self) {
    return TYPE_CLASS(slice);
}