#include "value_none.h"
#include "value_int.h"
// #include "methods_none.h"

Value None_Equal(Value a, Value b) {
    return BOOL_VAL(b.type == VAL_NONE);
}

Value None_NotEqual(Value a, Value b) {
    return BOOL_VAL(b.type != VAL_NONE);
}

Value None_GetAttr(Value value, ObjString *name) {
    // return getGperfMethod(value, name, in_none_set);
}

Value None_Class(Value value) {
    // return TYPE_CLASS(none);
}

uint64_t None_Hash(Value value) {
    return 4238894112;
}

bool None_ToBool(Value value) {
    return false;
}

int None_ToStr(Value value, char *buffer, size_t size) {
    writeToBuffer(buffer, size, "None");
}