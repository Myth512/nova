#include "value.h"
#include "value_int.h"
#include "value_float.h"
#include "vm.h"

Value floatEqual(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_FLOAT(a) == AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_FLOAT(a) == AS_FLOAT(b));
    return BOOL_VAL(false);
}

Value floatNotEqual(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_FLOAT(a) != AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_FLOAT(a) != AS_FLOAT(b));
    return BOOL_VAL(true);
}

Value floatGreater(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_FLOAT(a) > AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_FLOAT(a) > AS_FLOAT(b));
    operatorNotImplemented(">", a, b);
}

Value floatGreaterEqual(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_FLOAT(a) >= AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_FLOAT(a) >= AS_FLOAT(b));
    operatorNotImplemented(">=", a, b);
}

Value floatLess(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_FLOAT(a) < AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_FLOAT(a) < AS_FLOAT(b));
    operatorNotImplemented("<", a, b);
}

Value floatLessEqual(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_FLOAT(a) <= AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_FLOAT(a) <= AS_FLOAT(b));
    operatorNotImplemented("<=", a, b);
}

Value floatAdd(Value a, Value b) {
    if (b.type == VAL_INT)
        return FLOAT_VAL(AS_FLOAT(a) + AS_INT(b));
    if (b.type == VAL_FLOAT)
        return FLOAT_VAL(AS_FLOAT(a) + AS_FLOAT(b));
    operatorNotImplemented("+", a, b);
}

Value floatSubtract(Value a, Value b) {
    if (b.type == VAL_INT)
        return FLOAT_VAL(AS_FLOAT(a) - AS_INT(b));
    if (b.type == VAL_FLOAT)
        return FLOAT_VAL(AS_FLOAT(a) - AS_FLOAT(b));
    operatorNotImplemented("*", a, b);
}

Value floatMultiply(Value a, Value b) {
    if (b.type == VAL_INT)
        return FLOAT_VAL(AS_FLOAT(a) * AS_INT(b));
    if (b.type == VAL_FLOAT)
        return FLOAT_VAL(AS_FLOAT(a) * AS_FLOAT(b));
    operatorNotImplemented("*", a, b);
}

Value floatTrueDivide(Value a, Value b) {
    if (b.type == VAL_INT) {
        if (AS_INT(b) == 0)
            reportRuntimeError("Division by zero");
        return FLOAT_VAL(AS_FLOAT(a) / AS_INT(b));
    }
    if (b.type == VAL_FLOAT) {
        if (AS_FLOAT(b) == 0)
            reportRuntimeError("Division by zero");
        return FLOAT_VAL(AS_FLOAT(a) / AS_FLOAT(b));
    }
    operatorNotImplemented("/", a, b);
}

Value floatFloorDivide(Value a, Value b) {
    if (b.type == VAL_INT) {
        if (AS_INT(b) == 0)
            reportRuntimeError("Division by zero");
        return FLOAT_VAL((long long)(AS_FLOAT(a) / AS_INT(b)));
    }
    if (b.type == VAL_FLOAT) {
        if (AS_FLOAT(b) == 0)
            reportRuntimeError("Division by zero");
        return FLOAT_VAL((long long)(AS_FLOAT(a) / AS_FLOAT(b)));
    }
    operatorNotImplemented("//", a, b);
}

Value floatModulo(Value a, Value b) {
    if (b.type == VAL_INT) {
        if (AS_INT(b) == 0)
            reportRuntimeError("Division by zero");
        return FLOAT_VAL(fmod(AS_FLOAT(a), AS_INT(b)));
    }
    if (b.type == VAL_FLOAT) {
        if (AS_FLOAT(b) == 0)
            reportRuntimeError("Division by zero");
        return FLOAT_VAL(fmod(AS_FLOAT(a), AS_FLOAT(b)));
    }
    operatorNotImplemented("%%", a, b);
}

Value floatPower(Value a, Value b) {
    if (b.type == VAL_INT)
        return FLOAT_VAL(pow(AS_FLOAT(a), AS_INT(b)));
    if (b.type == VAL_FLOAT)
        return FLOAT_VAL(pow(AS_FLOAT(a), AS_FLOAT(b)));
    operatorNotImplemented("**", a, b);
}

Value floatPositive(Value a) {
    return a;
}

Value floatNegative(Value a) {
    return FLOAT_VAL(-AS_FLOAT(a));
}

uint64_t floatHash(Value value) {
    union {
        double d;
        uint64_t i;
    } u;
    u.d = AS_FLOAT(value);

    uint64_t hash = u.i;
    hash ^= (hash >> 33);
    hash *= 0xff51afd7ed558ccd;
    hash ^= (hash >> 33);
    hash *= 0xc4ceb9fe1a85ec53;
    hash ^= (hash >> 33);

    return hash;
}

bool floatToBool(Value value) {
    return AS_FLOAT(value);
}

long long floatToInt(Value value) {
    return AS_FLOAT(value);
}

double floatToFloat(Value value) {
    return AS_FLOAT(value);
}

ObjString *floatToStr(Value value) {

}

ObjString *floatToRepr(Value value) {

}