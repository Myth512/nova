#include <math.h>

#include "value.h"
#include "value_int.h"
#include "value_float.h"
#include "value_methods.h"
#include "methods_int.h"
#include "object_exception.h"
#include "vm.h"
#include "table.h"

Value Int_Equal(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return BOOL_VAL(AS_INT(a) == AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_NotEqual(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return BOOL_VAL(AS_INT(a) != AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_Greater(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return BOOL_VAL(AS_INT(a) > AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_GreaterEqual(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return BOOL_VAL(AS_INT(a) >= AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_Less(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return BOOL_VAL(AS_INT(a) < AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_LessEqual(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return BOOL_VAL(AS_INT(a) <= AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_Add(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) + AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_Subtract(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) - AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_RightSubtract(Value a, Value b) {
    return Int_Subtract(b, a);
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(b) - AS_INT(a));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_Multiply(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) * AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_TrueDivide(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL) {
        if (AS_INT(b) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return FLOAT_VAL((double)AS_INT(a) / AS_INT(b));
    }
    return NOT_IMPLEMENTED_VAL;
}

Value Int_RightTrueDivide(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL) {
        if (AS_INT(a) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return FLOAT_VAL(AS_INT(b) / AS_INT(a));
    }
    return NOT_IMPLEMENTED_VAL;
}

Value Int_FloorDivide(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL) {
        if (AS_INT(b) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return INT_VAL(AS_INT(a) / AS_INT(b));
    }
    return NOT_IMPLEMENTED_VAL;
}

Value Int_RightFloorDivide(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL) {
        if (AS_INT(a) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return INT_VAL(AS_INT(b) / AS_INT(a));
    }
    return NOT_IMPLEMENTED_VAL;
}

Value Int_Modulo(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL) {
        if (AS_INT(b) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return INT_VAL(AS_INT(a) % AS_INT(b));
    }
    return NOT_IMPLEMENTED_VAL;
}

Value Int_RightModulo(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL) {
        if (AS_INT(a) == 0)
            return createMsgException("Division by zero", VAL_ZERO_DIVISON_ERROR);
        return INT_VAL(AS_INT(b) % AS_INT(a));
    }
    return NOT_IMPLEMENTED_VAL;
}

Value Int_Power(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(pow(AS_INT(a), AS_INT(b)));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_RightPower(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(pow(AS_INT(b), AS_INT(a)));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_Positive(Value a) {
    return a; 
}

Value Int_Negative(Value a) {
    return INT_VAL(-AS_INT(a));
}

Value Int_And(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) & AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_Xor(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) ^ AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_Or(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) | AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_Invert(Value a) {
    return INT_VAL(~AS_INT(a));
}

Value Int_LeftShift(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) << AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_RightLeftShift(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(b) << AS_INT(a));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_RightShift(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(a) >> AS_INT(b));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_RightRightShift(Value a, Value b) {
    if (b.type == VAL_INT || b.type == VAL_BOOL)
        return INT_VAL(AS_INT(b) >> AS_INT(a));
    return NOT_IMPLEMENTED_VAL;
}

Value Int_GetAttr(Value obj, ObjString *name) {
    return getGperfMethod(obj, name, in_int_set);
}

Value Int_Init(Value callee, int argc, Value *argv) {
    return INT_VAL(valueToInt(argv[0]));
}

Value Bool_Init(Value callee, int argc, Value *argv) {
    return BOOL_VAL(valueToBool(argv[0]));
}

Value Int_Class(Value value) {
    return TYPE_CLASS(int_);
}

Value Bool_Class(Value value) {
    return TYPE_CLASS(bool_);
}

uint64_t Int_Hash(Value value) {
    uint64_t hash = (uint64_t)AS_INT(value);
    
    hash ^= (hash >> 33);
    hash *= 0xff51afd7ed558ccd;
    hash ^= (hash >> 33);
    hash *= 0xc4ceb9fe1a85ec53;
    hash ^= (hash >> 33);

    return hash;
}

bool Int_ToBool(Value value) {
    return AS_INT(value);
}

long long Int_ToInt(Value value) {
    return AS_INT(value);
}

double Int_ToFloat(Value value) {
    return AS_INT(value);
}

int Int_ToStr(Value value, char *buffer, const size_t size) {
    writeToBuffer(buffer, size, "%lld", AS_INT(value));
}

int Bool_ToStr(Value value, char *buffer, const size_t size) {
    writeToBuffer(buffer, size, "%s", AS_BOOL(value) ? "True" : "False");
}