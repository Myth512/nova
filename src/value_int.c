#include <math.h>

#include "value_int.h"
#include "value_float.h"
#include "vm.h"

Value intEqual(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_INT(a) == AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_INT(a) == AS_FLOAT(b));
    return BOOL_VAL(false);
}

Value intNotEqual(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return BOOL_VAL(AS_INT(a) != AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_INT(a) != AS_FLOAT(b));
    return BOOL_VAL(true);
}

Value intGreater(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return BOOL_VAL(AS_INT(a) > AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_INT(a) > AS_FLOAT(b));
    operatorNotImplemented(">", a, b);
}

Value intGreaterEqual(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_INT(a) >= AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_INT(a) >= AS_FLOAT(b));
    operatorNotImplemented(">=", a, b);
}

Value intLess(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_INT(a) < AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_INT(a) < AS_FLOAT(b));
    operatorNotImplemented("<", a, b);
}

Value intLessEqual(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_INT(a) <= AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_INT(a) <= AS_FLOAT(b));
    operatorNotImplemented("<=", a, b);
}

Value intAdd(Value a, Value b) {
    if (b.type == VAL_INT)
        return INT_VAL(AS_INT(a) + AS_INT(b));
    if (b.type == VAL_FLOAT)
        return FLOAT_VAL(AS_INT(a) + AS_FLOAT(b));
    operatorNotImplemented("+", a, b);
}

Value intSubtract(Value a, Value b) {
    if (b.type == VAL_INT)
        return INT_VAL(AS_INT(a) - AS_INT(b));
    if (b.type == VAL_FLOAT)
        return FLOAT_VAL(AS_INT(a) - AS_FLOAT(b));
    operatorNotImplemented("-", a, b);
}

Value intMultiply(Value a, Value b) {
    if (b.type == VAL_INT)
        return INT_VAL(AS_INT(a) * AS_INT(b));
    if (b.type == VAL_FLOAT)
        return FLOAT_VAL(AS_INT(a) * AS_FLOAT(b));
    operatorNotImplemented("*", a, b);
}

Value intTrueDivide(Value a, Value b) {
    if (b.type == VAL_INT) {
        if (AS_INT(b) == 0)
            reportRuntimeError("Division by zero");
        return FLOAT_VAL(AS_INT(a) / AS_INT(b));
    }
    if (b.type == VAL_FLOAT) {
        if (AS_FLOAT(b) == 0)
            reportRuntimeError("Division by zero");
        return FLOAT_VAL(AS_INT(a) / AS_FLOAT(b));
    }
    operatorNotImplemented("/", a, b);
}

Value intFloorDivide(Value a, Value b) {
    if (b.type == VAL_INT) {
        if (AS_INT(b) == 0)
            reportRuntimeError("Division by zero");
        return INT_VAL(AS_INT(a) / AS_INT(b));
    }
    if (b.type == VAL_FLOAT) {
        if (AS_FLOAT(b) == 0)
            reportRuntimeError("Division by zero");
        return FLOAT_VAL((long long)(AS_INT(a) / AS_FLOAT(b)));
    }
    operatorNotImplemented("//", a, b);
}

Value intModulo(Value a, Value b) {
    if (b.type == VAL_INT) {
        if (AS_INT(b) == 0)
            reportRuntimeError("Division by zero");
        return INT_VAL(AS_INT(a) % AS_INT(b));
    }
    if (b.type == VAL_FLOAT) {
        if (AS_FLOAT(b) == 0)
            reportRuntimeError("Division by zero");
        return FLOAT_VAL(fmod(AS_INT(a), AS_FLOAT(b)));
    }
    operatorNotImplemented("%%", a, b);
}

Value intPower(Value a, Value b) {
    if (b.type == VAL_INT)
        return INT_VAL(pow(AS_INT(a), AS_INT(b)));
    if (b.type == VAL_FLOAT)
        return FLOAT_VAL(pow(AS_INT(a), AS_FLOAT(b)));
    operatorNotImplemented("**", a, b);
}

Value intPositive(Value a) {
    return a; 
}

Value intNegative(Value a) {
    return INT_VAL(-AS_INT(a));
}

Value intAnd(Value a, Value b) {
    if (b.type == VAL_INT)
        return INT_VAL(AS_INT(a) & AS_INT(b));
    operatorNotImplemented("&", a, b);
}

Value intXor(Value a, Value b) {
    if (b.type == VAL_INT)
        return INT_VAL(AS_INT(a) ^ AS_INT(b));
    operatorNotImplemented("^", a, b);
}

Value intOr(Value a, Value b) {
    if (b.type == VAL_INT)
        return INT_VAL(AS_INT(a) | AS_INT(b));
    operatorNotImplemented("|", a, b);
}

Value intInvert(Value a) {
    return INT_VAL(~AS_INT(a));
}

Value intLeftShift(Value a, Value b) {
    if (b.type == VAL_INT)
        return INT_VAL(AS_INT(a) << AS_INT(b));
    operatorNotImplemented("<<", a, b);
}

Value intRightShift(Value a, Value b) {
    if (b.type == VAL_INT)
        return INT_VAL(AS_INT(a) >> AS_INT(b));
    operatorNotImplemented(">>", a, b);
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