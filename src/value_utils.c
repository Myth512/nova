#include "value_utils.h"
#include "value_none.h"
#include "value_int.h"
#include "value_float.h"
#include "object_string.h"
#include "vm.h"

typedef Value (*BinaryMethod)(Value, Value);
typedef Value (*UnaryMethod)(Value);

#define GET_UNARY_METHOD(type, name) (UnaryMethod)checkForNull((void*)MethodTable[type].name)
#define GET_METHOD(value, name) MethodTable[(value).type].name

typedef struct {
    BinaryMethod eq;
    BinaryMethod ne;
    BinaryMethod gt;
    BinaryMethod ge;
    BinaryMethod lt;
    BinaryMethod le;
    BinaryMethod add;
    BinaryMethod sub;
    BinaryMethod mul;
    BinaryMethod truediv;
    BinaryMethod floordiv;
    BinaryMethod mod;
    BinaryMethod pow;
    UnaryMethod pos;
    UnaryMethod neg;
    BinaryMethod and;
    BinaryMethod xor;
    BinaryMethod or;
    UnaryMethod invert;
    BinaryMethod lshift;
    BinaryMethod rshift;
    uint64_t (*hash)(Value);
    long long (*len)(Value);
    bool (*toBool)(Value);
    long long (*toInt)(Value);
    double (*toFloat)(Value);
    ObjString (*toStr)(Value);
    ObjString (*toRepr)(Value);
} ValueMethods;

const ValueMethods MethodTable[] = {
    [VAL_NONE]   = {noneEqual,   noneNotEqual,   NULL,          NULL,               NULL,       NULL,            NULL,      NULL,          NULL,            NULL,             NULL,             NULL,        NULL,       NULL,          NULL,          NULL,   NULL,   NULL,  NULL,      NULL,         NULL,          noneHash,   NULL,      noneToBool,  noneToInt,    noneToFloat,   NULL,       NULL},
    [VAL_BOOL]   = {intEqual,    intNotEqual,    intGreater,    intGreaterEqual,    intLess,    intLessEqual,    intAdd,    intSubtract,   intMultiply,     intTrueDivide,    intFloorDivide,   intModulo,   intPower,   intPositive,   intNegative,   intAnd, intXor, intOr, intInvert, intLeftShift, intRightShift, intHash,    NULL,      intToBool,   intToInt,     intToFloat,    NULL,       NULL},
    [VAL_INT]    = {intEqual,    intNotEqual,    intGreater,    intGreaterEqual,    intLess,    intLessEqual,    intAdd,    intSubtract,   intMultiply,     intTrueDivide,    intFloorDivide,   intModulo,   intPower,   intPositive,   intNegative,   intAnd, intXor, intOr, intInvert, intLeftShift, intRightShift, intHash,    NULL,      intToBool,   intToInt,     intToFloat,    intToStr,   intToRepr},
    [VAL_FLOAT]  = {floatEqual,  floatNotEqual,  floatGreater,  floatGreaterEqual,  floatLess,  floatLessEqual,  floatAdd,  floatSubtract, floatMultiply,   floatTrueDivide,  floatFloorDivide, floatModulo, floatPower, floatPositive, floatNegative, NULL,   NULL,   NULL,  NULL,      NULL,         NULL,          floatHash,  NULL,      floatToBool, floatToInt,   floatToFloat,  floatToStr, floatToRepr},
    [VAL_STRING] = {stringEqual, stringNotEqual, stringGreater, stringGreaterEqual, stringLess, stringLessEqual, stringAdd, NULL,          stringMultiply,  NULL,             NULL,             NULL,        NULL,       NULL,          NULL,          NULL,   NULL,   NULL,  NULL,      NULL,         NULL,          stringHash, stringLen, stringToBool, stringToInt, stringToFloat, NULL,       NULL}
};

void *checkForNull(void *p) {
    if (p == NULL)
        reportRuntimeError("Operation is not supported");
    return p;
}

Value unaryMethod(Value a, UnaryMethod method, char *name) {
    if (method != 0) {
        Value res = method(a);
        if (!IS_NOT_IMPLEMENTED(res))
            return res;
    }
    operatorNotImplementedUnary(name, a);
}

Value binaryMethod(Value a, Value b, BinaryMethod left, BinaryMethod right, char *name) {
    if (left != NULL) {
        Value res = left(a, b);
        if (!IS_NOT_IMPLEMENTED(res))
            return res;
    }
    if (right != NULL) {
        Value res = right(b, a);
        if (!IS_NOT_IMPLEMENTED(res))
            return res;
    }
    operatorNotImplemented(name, a, b);
}

Value valueEqual(Value a, Value b) {
    BinaryMethod left = GET_METHOD(a, eq);
    if (left != NULL) {
        Value res = left(a, b);
        if (!IS_NOT_IMPLEMENTED(res))
            return res;
    }
    BinaryMethod right = GET_METHOD(b, eq);
    if (right != NULL) {
        Value res = right(b, a);
        if (!IS_NOT_IMPLEMENTED(res))
            return res;
    }
    return BOOL_VAL(valueId(a) == valueId(b));
}

Value valueNotEqual(Value a, Value b) {
    BinaryMethod left = GET_METHOD(a, ne);
    if (left != NULL) {
        Value res = left(a, b);
        if (!IS_NOT_IMPLEMENTED(res))
            return res;
    }
    BinaryMethod right = GET_METHOD(b, ne);
    if (right != NULL) {
        Value res = right(b, a);
        if (!IS_NOT_IMPLEMENTED(res))
            return res;
    }
    return BOOL_VAL(valueId(a) != valueId(b));
}

Value valueGreater(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, gt), GET_METHOD(b, lt), ">");
}

Value valueGreaterEqual(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, ge), GET_METHOD(b, le), ">=");
}

Value valueLess(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, lt), GET_METHOD(b, gt), "<");
}

Value valueLessEqual(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, le), GET_METHOD(b, ge), "<=");
}

Value valueAdd(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, add), GET_METHOD(b, add), "+");
}

Value valueSubtract(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, sub), GET_METHOD(b, sub), "-");
}

Value valueMultiply(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, mul), GET_METHOD(b, mul), "*");
}

Value valueTrueDivide(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, truediv), GET_METHOD(b, truediv), "/");
}

Value valueFloorDivide(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, floordiv), GET_METHOD(b, floordiv), "//");
}

Value valueModulo(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, mod), GET_METHOD(b, mod), "%%");
}

Value valuePower(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, pow), GET_METHOD(b, pow), "**");
}

Value valuePositive(Value a) {
    return unaryMethod(a, GET_METHOD(a, pos), '+');
}

Value valueNegative(Value a) {
    return unaryMethod(a, GET_METHOD(a, neg), '-');
}

Value valueAnd(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, and), GET_METHOD(b, and), "&");
}

Value valueXor(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, xor), GET_METHOD(b, xor), "^");
}

Value valueOr(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, or), GET_METHOD(b, or), "|");
}

Value valueInvert(Value a) {
    return unaryMethod(a, GET_METHOD(a, invert), '~');
}

Value valueLeftShift(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, lshift), GET_METHOD(b, lshift), "<<");
}

Value valueRightShift(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, rshift), GET_METHOD(b, rshift), ">>");
}

OptValue valueGetAttribute(Value obj, ObjString *name) {

}

void valueSetAttribute(Value obj, ObjString *name, Value value) {

}

void valueDelAttribute(Value obj, ObjString *name) {

}

Value valueGetItem(Value obj, Value key) {

}

void valueSetItem(Value obj, Value key, Value value) {

}

void valueDelItem(Value obj, Value key) {

}

Value valueCall(Value obj) {

}

uint64_t valueHash(Value value) {
    uint64_t (*method)(Value) = checkForNull(MethodTable[value.type].hash);
    return method(value);
}

uint64_t valueId(Value value) {
    switch (value.type) {
        case VAL_NONE:
        case VAL_BOOL:
        case VAL_INT:
        case VAL_FLOAT:
        case VAL_UNDEFINED:
        case VAL_NOT_IMPLEMENTED:
            return &value;
        default:
            return value.as.obj;
    }
}

long long valueLen(Value value) {
    long long (*method)(Value) = checkForNull(MethodTable[value.type].len);
    return method(value);
}

bool valueToBool(Value value) {
    bool (*method)(Value) = checkForNull(MethodTable[value.type].toBool);
    return method(value);
}

long long valueToInt(Value value) {
    long long (*method)(Value) = checkForNull(MethodTable[value.type].toInt);
    return method(value);
}

double valueToFloat(Value value) {
    double (*method)(Value) = checkForNull(MethodTable[value.type].toFloat);
    return method(value);
}

ObjString *valueToStr(Value value) {
    ObjString *(*method)(Value) = checkForNull(MethodTable[value.type].toStr);
    return method(value);
}

ObjString *valueToRepr(Value value) {
    ObjString *(*method)(Value) = checkForNull(MethodTable[value.type].toRepr);
    return method(value);
}