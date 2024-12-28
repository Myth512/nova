#include "value_utils.h"
#include "value_int.h"
#include "value_float.h"
#include "vm.h"

typedef Value (*BinaryMethod)(Value, Value);

#define GET_BINARY_METHOD(type, name) (BinaryMethod)checkForNull((void*)MethodTable[type].name)

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

    BinaryMethod and;
    BinaryMethod xor;
    BinaryMethod or;
    BinaryMethod lshift;
    BinaryMethod rshift;
} ValueMethods;

ValueMethods MethodTable[] = {
    [VAL_INT] = {intEqual, intNotEqual, intGreater, intGreaterEqual, intLess, intLessEqual, intAdd, intSubtract, intMultiply, intTrueDivide, intFloorDivide, intModulo, intPower},
    [VAL_FLOAT] = {floatEqual, floatNotEqual, floatGreater, floatGreaterEqual, floatLess, floatLessEqual, floatSubtract, floatMultiply, floatTrueDivide, floatFloorDivide, floatModulo, floatPower}
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