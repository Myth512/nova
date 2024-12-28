#include "value_none.h"

Value noneEqual(Value a, Value b) {
    return BOOL_VAL(b.type == VAL_NONE);
}

Value noneNotEqual(Value a, Value b) {
    return BOOL_VAL(b.type != VAL_NONE);
}

uint64_t noneHash(Value value) {
    return 4238894112;
}

bool noneToBool(Value value) {
    return false;
}

long long noneToInt(Value value) {
    return 0;
}

double noneToFloat(Value value) {
    return 0;
}