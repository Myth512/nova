#include "value_methods.h"
#include "value_none.h"
#include "value_int.h"
#include "value_float.h"
#include "object_string.h"
#include "object_class.h"
#include "object_instance.h"
#include "vm.h"

#define GET_METHOD(value, name) MethodTable[(value).type].name

ValueMethods MethodTable[] = {
    [VAL_NONE]   = NONE_METHODS, 
    [VAL_BOOL]   = BOOL_METHODS,
    [VAL_INT]    = INT_METHODS, 
    [VAL_FLOAT]  = FLOAT_METHODS,
    [VAL_STRING] = STRING_METHODS,
    [VAL_LIST] = LIST_METHODS,
    [VAL_NATIVE] = NATIVE_METHODS,
    [VAL_FUNCTION] = FUNCTION_METHODS,
    [VAL_CLOSURE] = CLOSURE_METHODS,
    [VAL_CLASS] = CLASS_METHODS,
    [VAL_NATIVE_CLASS] = NATIVE_CLASS_METHODS,
    [VAL_METHOD] = MEHTOD_METHODS,
    [VAL_NATIVE_METHOD] = NATIVE_METHOD_METHODS,
    [VAL_INSTANCE] = INSTANCE_METHODS
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
    return unaryMethod(a, GET_METHOD(a, pos), "+");
}

Value valueNegative(Value a) {
    return unaryMethod(a, GET_METHOD(a, neg), "-");
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
    return unaryMethod(a, GET_METHOD(a, invert), "~");
}

Value valueLeftShift(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, lshift), GET_METHOD(b, lshift), "<<");
}

Value valueRightShift(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, rshift), GET_METHOD(b, rshift), ">>");
}

Value valueGetAttr(Value obj, ObjString *name) {
    Value (*method)(Value, ObjString*) = GET_METHOD(obj, getattr);
    return method(obj, name);
}

void valueSetAttr(Value obj, ObjString *name, Value value) {
    Value (*method)(Value, ObjString*, Value) = GET_METHOD(obj, setattr);
    if (method == NULL)
        reportRuntimeError("read only");
    method(obj, name, value);
}

void valueDelAttr(Value obj, ObjString *name) {
    Value (*method)(Value, ObjString*) = GET_METHOD(obj, delattr);
    if (method == NULL)
        reportRuntimeError("read only");
    method(obj, name);
}

Value valueGetItem(Value obj, Value key) {
    Value (*method)(Value, Value) = GET_METHOD(obj, getitem);
    if (method == NULL)
        reportRuntimeError("not subscriptable");
    method(obj, key);
}

void valueSetItem(Value obj, Value key, Value value) {
    Value (*method)(Value, Value, Value) = GET_METHOD(obj, setitem);
    if (method == NULL)
        reportRuntimeError("not subscriptable");
    method(obj, key, value);
}

void valueDelItem(Value obj, Value key) {
    Value (*method)(Value, Value) = GET_METHOD(obj, delitem);
    if (method == NULL)
        reportRuntimeError("not item deletion");
    method(obj, key);
}

Value valueInit(Value callee, int argc, Value *argv) {
    Value (*method)(Value, int, Value*) = GET_METHOD(callee, init);
    return method(callee, argc, argv);
}

Value valueCall(Value callee, int argc, Value *argv) {
    Value (*method)(Value, int, Value*) = GET_METHOD(callee, call);
    if (method == NULL)
        reportRuntimeError("no call :(");
    return method(callee, argc, argv);
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
            return (uint64_t)&value;
        default:
            return (uint64_t)value.as.object;
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

int valueWrite(Value value, char *buffer, size_t size) {
    int (*str)(Value, char*, size_t) = GET_METHOD(value, str);
    return str(value, buffer, size);
}

int valuePrint(Value value) {
    int (*str)(Value, char*, size_t) = GET_METHOD(value, str);
    if (str == NULL)
        return printf("TODO");
    return str(value, NULL, 0);
}

int valueRepr(Value value) {
    int (*repr)(Value, char*, size_t) = GET_METHOD(value, repr);
    if (repr == NULL)
        return printf("TODO");
    return repr(value, NULL, 0);
}

ObjString *valueToStr(Value value) {
    const size_t size = 256;
    char buffer[size];

    int (*str)(Value, char*, size_t) = GET_METHOD(value, str);
    int length = str(value, buffer, size);

    ObjString *string = copyString(buffer, length);
    return string;
}

ObjString *valueToRepr(Value value) {
    const size_t size = 256;
    char buffer[size];

    int (*repr)(Value, char*, size_t) = GET_METHOD(value, repr);
    int length = repr(value, buffer, size);

    ObjString *string = copyString(buffer, length);
    return string;
}