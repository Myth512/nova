#include "value_utils.h"
#include "value_none.h"
#include "value_int.h"
#include "value_float.h"
#include "vm.h"

typedef Value (*BinaryMethod)(Value, Value);
typedef Value (*UnaryMethod)(Value);

#define GET_BINARY_METHOD(type, name) (BinaryMethod)checkForNull((void*)MethodTable[type].name)
#define GET_UNARY_METHOD(type, name) (UnaryMethod)checkForNull((void*)MethodTable[type].name)

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
    [VAL_NONE] =  {noneEqual,  noneNotEqual,  NULL,         NULL,              NULL,      NULL,           NULL,     NULL,          NULL,            NULL,             NULL,             NULL,        NULL,       NULL,          NULL,          NULL,   NULL,   NULL,  NULL,      NULL,         NULL,          noneHash,  NULL, noneToBool,  noneToInt,  noneToFloat,  NULL,       NULL},
    [VAL_BOOL] =  {intEqual,   intNotEqual,   intGreater,   intGreaterEqual,   intLess,   intLessEqual,   intAdd,   intSubtract,   intMultiply,     intTrueDivide,    intFloorDivide,   intModulo,   intPower,   intPositive,   intNegative,   intAnd, intXor, intOr, intInvert, intLeftShift, intRightShift, intHash,   NULL, intToBool,   intToInt,   intToFloat,   NULL,       NULL},
    [VAL_INT] =   {intEqual,   intNotEqual,   intGreater,   intGreaterEqual,   intLess,   intLessEqual,   intAdd,   intSubtract,   intMultiply,     intTrueDivide,    intFloorDivide,   intModulo,   intPower,   intPositive,   intNegative,   intAnd, intXor, intOr, intInvert, intLeftShift, intRightShift, intHash,   NULL, intToBool,   intToInt,   intToFloat,   intToStr,   intToRepr},
    [VAL_FLOAT] = {floatEqual, floatNotEqual, floatGreater, floatGreaterEqual, floatLess, floatLessEqual, floatAdd, floatSubtract, floatMultiply,   floatTrueDivide,  floatFloorDivide, floatModulo, floatPower, floatPositive, floatNegative, NULL,   NULL,   NULL,  NULL,      NULL,         NULL,          floatHash, NULL, floatToBool, floatToInt, floatToFloat, floatToStr, floatToRepr}
};

void *checkForNull(void *p) {
    if (p == NULL)
        reportRuntimeError("Operation is not supported");
    return p;
}

Value valueEqual(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, eq);
    return method(a, b);
}

Value valueNotEqual(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, ne);
    return method(a, b);
}

Value valueGreater(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, gt);
    return method(a, b);
}

Value valueGreaterEqual(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, ge);
    return method(a, b);
}

Value valueLess(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, lt);
    return method(a, b);
}

Value valueLessEqual(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, le);
    return method(a, b);
}

Value valueAdd(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, add);
    return method(a, b);
}

Value valueSubtract(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, sub);
    return method(a, b);
}

Value valueMultiply(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, mul);
    return method(a, b);
}

Value valueTrueDivide(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, truediv);
    return method(a, b);
}

Value valueFloorDivide(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, floordiv);
    return method(a, b);
}

Value valueModulo(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, mod);
    return method(a, b);
}

Value valuePower(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, pow);
    return method(a, b);
}

Value valuePositive(Value a) {
    UnaryMethod method = GET_UNARY_METHOD(a.type, pos);
    return method(a);
}

Value valueNegative(Value a) {
    UnaryMethod method = GET_UNARY_METHOD(a.type, neg);
    return method(a);
}

Value valueAnd(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, and);
    return method(a, b);
}

Value valueXor(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, xor);
    return method(a, b);
}

Value valueOr(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, or);
    return method(a, b);
}

Value valueInvert(Value a) {
    UnaryMethod method = GET_UNARY_METHOD(a.type, invert);
    return method(a);
}

Value valueLeftShift(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, lshift);
    return method(a, b);
}

Value valueRightShift(Value a, Value b) {
    BinaryMethod method = GET_BINARY_METHOD(a.type, rshift);
    return method(a, b);
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