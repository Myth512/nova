#include <math.h>

#include "value_int.h"
#include "value_float.h"
#include "vm.h"

Value intEqual(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return BOOL_VAL(AS_INT(a) == AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value intNotEqual(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return BOOL_VAL(AS_INT(a) != AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value intGreater(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return BOOL_VAL(AS_INT(a) > AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value intGreaterEqual(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return BOOL_VAL(AS_INT(a) >= AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value intLess(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return BOOL_VAL(AS_INT(a) < AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value intLessEqual(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return BOOL_VAL(AS_INT(a) <= AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value intAdd(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) + AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value intSubtract(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) - AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value intMultiply(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) * AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value intTrueDivide(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL) {
        if (AS_INT(b) == 0)
            reportRuntimeError("Division by zero");
        return FLOAT_VAL(AS_INT(a) / AS_INT(b));
    }
    return NOT_IMPLEMENTED_VAL;
}

Value intFloorDivide(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL) {
        if (AS_INT(b) == 0)
            reportRuntimeError("Division by zero");
        return INT_VAL(AS_INT(a) / AS_INT(b));
    }
    return NOT_IMPLEMENTED_VAL;
}

Value intModulo(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL) {
        if (AS_INT(b) == 0)
            reportRuntimeError("Division by zero");
        return INT_VAL(AS_INT(a) % AS_INT(b));
    }
    return NOT_IMPLEMENTED_VAL;
}

Value intPower(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(pow(AS_INT(a), AS_INT(b)));
    return NOT_IMPLEMENTED_VAL;
}

Value intPositive(Value a) {
    return a; 
}

Value intNegative(Value a) {
    return INT_VAL(-AS_INT(a));
}

Value intAnd(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) & AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value intXor(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) ^ AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value intOr(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) | AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value intInvert(Value a) {
    return INT_VAL(~AS_INT(a));
}

Value intLeftShift(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) << AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value intRightShift(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) >> AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

uint64_t intHash(Value value) {
    uint64_t hash = (uint64_t)AS_INT(value);
    
    hash ^= (hash >> 33);
    hash *= 0xff51afd7ed558ccd;
    hash ^= (hash >> 33);
    hash *= 0xc4ceb9fe1a85ec53;
    hash ^= (hash >> 33);

    return hash;
}

bool intToBool(Value value) {
    return AS_INT(value);
}

long long intToInt(Value value) {
    return AS_INT(value);
}

double intToFloat(Value value) {
    return AS_INT(value);
}

ObjString *intToStr(Value value) {

}

ObjString *intToRepr(Value value) {

}