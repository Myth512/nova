#include "value_none.h"
#include "value_int.h"

Value None_Equal(Value a, Value b) {
    return BOOL_VAL(b.type == VAL_NONE);
}

Value None_NotEqual(Value a, Value b) {
    return BOOL_VAL(b.type != VAL_NONE);
}

uint64_t None_Hash(Value value) {
    return 4238894112;
}

bool None_ToBool(Value value) {
    return false;
}

long long None_ToInt(Value value) {
    return 0;
}

double None_ToFloat(Value value) {
    return 0;
}

int None_ToStr(Value value, char *buffer, size_t size) {
    writeToBuffer(buffer, size, "None");
}