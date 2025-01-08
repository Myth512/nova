#include <math.h>

#include "value.h"
#include "value_int.h"
#include "value_float.h"
#include "value_methods.h"
#include "methods_float.h"
#include "object_exception.h"
#include "vm.h"

Value Float_Equal(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_FLOAT(a) == AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_FLOAT(a) == AS_FLOAT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Float_NotEqual(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_FLOAT(a) != AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_FLOAT(a) != AS_FLOAT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Float_Greater(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_FLOAT(a) > AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_FLOAT(a) > AS_FLOAT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Float_GreaterEqual(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_FLOAT(a) >= AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_FLOAT(a) >= AS_FLOAT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Float_Less(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_FLOAT(a) < AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_FLOAT(a) < AS_FLOAT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Float_LessEqual(Value a, Value b) {
    if (b.type == VAL_INT)
        return BOOL_VAL(AS_FLOAT(a) <= AS_INT(b));
    if (b.type == VAL_FLOAT)
        return BOOL_VAL(AS_FLOAT(a) <= AS_FLOAT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Float_Add(Value a, Value b) {
    if (b.type == VAL_INT)
        return FLOAT_VAL(AS_FLOAT(a) + AS_INT(b));
    if (b.type == VAL_FLOAT)
        return FLOAT_VAL(AS_FLOAT(a) + AS_FLOAT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Float_Subtract(Value a, Value b) {
    if (b.type == VAL_INT)
        return FLOAT_VAL(AS_FLOAT(a) - AS_INT(b));
    if (b.type == VAL_FLOAT)
        return FLOAT_VAL(AS_FLOAT(a) - AS_FLOAT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Float_RightSubtract(Value a, Value b) {
    if (b.type == VAL_INT)
        return FLOAT_VAL(AS_INT(b) - AS_FLOAT(a));
    if (b.type == VAL_FLOAT)
        return FLOAT_VAL(AS_FLOAT(b) - AS_FLOAT(a));
    return NOT_IMPLEMENTED_VAL;
}

Value Float_Multiply(Value a, Value b) {
    if (b.type == VAL_INT)
        return FLOAT_VAL(AS_FLOAT(a) * AS_INT(b));
    if (b.type == VAL_FLOAT)
        return FLOAT_VAL(AS_FLOAT(a) * AS_FLOAT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Float_TrueDivide(Value a, Value b) {
    if (b.type == VAL_INT) {
        if (AS_INT(b) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return FLOAT_VAL(AS_INT(a) / AS_FLOAT(b));
    }
    if (b.type == VAL_FLOAT) {
        if (AS_FLOAT(b) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return FLOAT_VAL(AS_FLOAT(a) / AS_FLOAT(b));
    }
    return NOT_IMPLEMENTED_VAL;
}

Value Float_RightTrueDivide(Value a, Value b) {
    if (b.type == VAL_INT) {
        if (AS_INT(a) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return FLOAT_VAL(AS_INT(b) / AS_FLOAT(a));
    }
    if (b.type == VAL_FLOAT) {
        if (AS_FLOAT(a) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return FLOAT_VAL(AS_FLOAT(b) / AS_FLOAT(a));
    }
    return NOT_IMPLEMENTED_VAL;
}

Value Float_FloorDivide(Value a, Value b) {
    if (b.type == VAL_INT) {
        if (AS_INT(b) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return FLOAT_VAL((long long)(AS_FLOAT(a) / AS_INT(b)));
    }
    if (b.type == VAL_FLOAT) {
        if (AS_FLOAT(b) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return FLOAT_VAL((long long)(AS_FLOAT(a) / AS_FLOAT(b)));
    }
    return NOT_IMPLEMENTED_VAL;
}

Value Float_RightFloorDivide(Value a, Value b) {
    if (b.type == VAL_INT) {
        if (AS_INT(a) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return FLOAT_VAL((long long)(AS_INT(b) / AS_FLOAT(a)));
    }
    if (b.type == VAL_FLOAT) {
        if (AS_FLOAT(a) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return FLOAT_VAL((long long)(AS_FLOAT(b) / AS_FLOAT(a)));
    }
    return NOT_IMPLEMENTED_VAL;
}

Value Float_Modulo(Value a, Value b) {
    if (b.type == VAL_INT) {
        if (AS_INT(b) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return FLOAT_VAL(fmod(AS_FLOAT(a), AS_INT(b)));
    }
    if (b.type == VAL_FLOAT) {
        if (AS_FLOAT(b) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return FLOAT_VAL(fmod(AS_FLOAT(a), AS_FLOAT(b)));
    }
    return NOT_IMPLEMENTED_VAL;
}

Value Float_RightModulo(Value a, Value b) {
    if (b.type == VAL_INT) {
        if (AS_INT(a) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return FLOAT_VAL(fmod(AS_INT(b), AS_FLOAT(a)));
    }
    if (b.type == VAL_FLOAT) {
        if (AS_FLOAT(a) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return FLOAT_VAL(fmod(AS_FLOAT(b), AS_FLOAT(a)));
    }
    return NOT_IMPLEMENTED_VAL;
}

Value Float_Power(Value a, Value b) {
    if (b.type == VAL_INT)
        return FLOAT_VAL(pow(AS_FLOAT(a), AS_INT(b)));
    if (b.type == VAL_FLOAT)
        return FLOAT_VAL(pow(AS_FLOAT(a), AS_FLOAT(b)));
    return NOT_IMPLEMENTED_VAL;
}

Value Float_RightPower(Value a, Value b) {
    if (b.type == VAL_INT)
        return FLOAT_VAL(pow(AS_INT(b), AS_FLOAT(a)));
    if (b.type == VAL_FLOAT)
        return FLOAT_VAL(pow(AS_FLOAT(b), AS_FLOAT(a)));
    return NOT_IMPLEMENTED_VAL;
}

Value Float_Positive(Value a) {
    return a;
}

Value Float_Negative(Value a) {
    return FLOAT_VAL(-AS_FLOAT(a));
}

Value Float_GetAttr(Value a, ObjString *name) {
    return getGperfMethod(a, name, in_float_set);
}

Value Float_Init(Value callee, int argc, Value *argv) {
    return FLOAT_VAL(valueToFloat(argv[0]));
}

Value Float_Class(Value value) {
    return OBJ_VAL(vm.types.float_);
}

uint64_t Float_Hash(Value value) {
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

bool Float_ToBool(Value value) {
    return AS_FLOAT(value);
}

long long Float_ToInt(Value value) {
    return AS_FLOAT(value);
}

double Float_ToFloat(Value value) {
    return AS_FLOAT(value);
}

int Float_ToStr(Value value, char *buffer, size_t size) {
    writeToBuffer(buffer, size, "%g", AS_FLOAT(value));
}