#include "value_methods.h"
#include "value_none.h"
#include "value_int.h"
#include "value_float.h"
#include "value_type.h"
#include "value_object.h"
#include "object_string.h"
#include "object_string_iterator.h"
#include "object_list.h"
#include "object_list_iterator.h"
#include "object_tuple.h"
#include "object_tuple_iterator.h"
#include "object_dict.h"
#include "object_dict_iterator.h"
#include "object_class.h"
#include "object_instance.h"
#include "object_super.h"
#include "object_range.h"
#include "object_range_iterator.h"
#include "object_exception.h"
#include "vm.h"

#define GET_METHOD(value, name) MethodTable[(value).type].name

static int Undefined_ToStr(Value value, char *buffer, size_t size) {
    return writeToBuffer(buffer, size, "undefined");
}

static int NotImplemened_ToStr(Value value, char *buffer, size_t size) {
    return writeToBuffer(buffer, size, "NotImplemented");
}

static Value NotImplemened_Class(Value value) {
    return TYPE_CLASS(notImplementedType);
}

ValueMethods MethodTable[] = {
    [VAL_UNDEFINED] = (ValueMethods){.str=Undefined_ToStr, .repr=Undefined_ToStr},
    [VAL_NOT_IMPLEMENTED] = (ValueMethods){.str=NotImplemened_ToStr, .repr=NotImplemened_ToStr, .class=NotImplemened_Class},
    [VAL_NONE]   = NONE_METHODS, 
    [VAL_BOOL]   = BOOL_METHODS,
    [VAL_INT]    = INT_METHODS, 
    [VAL_FLOAT]  = FLOAT_METHODS,
    [VAL_TYPE] = TYPE_METHODS,
    [VAL_STRING] = STRING_METHODS,
    [VAL_STRING_ITERATOR] = STRING_ITERATOR_METHODS,
    [VAL_LIST] = LIST_METHODS,
    [VAL_LIST_ITERATOR] = LIST_ITERATOR_METHODS,
    [VAL_TUPLE] = TUPLE_METHODS,
    [VAL_TUPLE_ITERATOR] = TUPLE_ITERATOR_METHODS,
    [VAL_DICT] = DICT_METHODS,
    [VAL_DICT_ITERATOR] = DICT_ITERATOR_METHODS,
    [VAL_NATIVE] = NATIVE_METHODS,
    [VAL_FUNCTION] = FUNCTION_METHODS,
    [VAL_CLOSURE] = CLOSURE_METHODS,
    [VAL_CLASS] = CLASS_METHODS,
    [VAL_NATIVE_CLASS] = NATIVE_CLASS_METHODS,
    [VAL_METHOD] = MEHTOD_METHODS,
    [VAL_NATIVE_METHOD] = NATIVE_METHOD_METHODS,
    [VAL_INSTANCE] = INSTANCE_METHODS,
    [VAL_SUPER] = SUPER_METHODS,
    [VAL_RANGE] = RANGE_METHODS,
    [VAL_RANGE_ITERATOR] = RANGE_ITERATOR_METHODS,
    [VAL_EXCEPTION] = EXCEPTION_METHODS,
    [VAL_ZERO_DIVISON_ERROR] = ZERO_DIVISON_ERROR_METHODS,
    [VAL_STOP_ITERATION] = STOP_ITERATION_METHODS,
    [VAL_TYPE_ERROR] = TYPE_ERROR_METHODS,
    [VAL_VALUE_ERROR] = VALUE_ERROR_METHODS,
    [VAL_INDEX_ERROR] = INDEX_ERROR_METHODS, 
    [VAL_KEY_ERROR] = KEY_ERROR_METHODS, 
    [VAL_ATTRIBUTE_ERROR] = ATTRIBUTE_ERROR_METHODS, 
    [VAL_NAME_ERROR] = NAME_ERROR_METHODS,
    [VAL_RUNTIME_ERROR] = RUNTIME_ERROR_METHODS,
    [VAL_ASSERTION_ERROR] = ASSERTION_ERROR_METHODS,
};

static void *getMethod(Value value, int offset) {
    void *method = &MethodTable[value.type] + offset;
    return method;
}

char *getValueType(Value value) {
    Value class = valueClass(value);
    if (IS_CLASS(class))
        return AS_CLASS(class)->name->chars;
    if (IS_NATIVE_CLASS(class))
        return AS_NATIVE_CLASS(class)->name->chars;
    return "unknown type";
}

Value unaryMethod(Value a, UnaryMethod method, char *name) {
    if (method != 0) {
        Value res = method(a);
        if (!IS_NOT_IMPLEMENTED(res))
            return res;
    }
    return createException(VAL_TYPE_ERROR, "bad operand type for unary %s: '%s'", name, getValueType(a));
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
    return createException(VAL_TYPE_ERROR, "unsupported operand type(s) for %s: '%s' and '%s'", name, getValueType(a), getValueType(b));
}

Value valueIs(Value a, Value b) {
    return BOOL_VAL(valueId(a) == valueId(b));
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
    return valueIs(a, b);
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
    return binaryMethod(a, b, GET_METHOD(a, add), GET_METHOD(b, radd), "+");
}

Value valueSubtract(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, sub), GET_METHOD(b, rsub), "-");
}

Value valueMultiply(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, mul), GET_METHOD(b, rmul), "*");
}

Value valueTrueDivide(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, truediv), GET_METHOD(b, rtruediv), "/");
}

Value valueFloorDivide(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, floordiv), GET_METHOD(b, rfloordiv), "//");
}

Value valueModulo(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, mod), GET_METHOD(b, rmod), "%%");
}

Value valuePower(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, pow), GET_METHOD(b, rpow), "**");
}

Value valuePositive(Value a) {
    return unaryMethod(a, GET_METHOD(a, pos), "+");
}

Value valueNegative(Value a) {
    return unaryMethod(a, GET_METHOD(a, neg), "-");
}

Value valueAnd(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, and), GET_METHOD(b, rand), "&");
}

Value valueXor(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, xor), GET_METHOD(b, rxor), "^");
}

Value valueOr(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, or), GET_METHOD(b, ror), "|");
}

Value valueInvert(Value a) {
    return unaryMethod(a, GET_METHOD(a, invert), "~");
}

Value valueLeftShift(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, lshift), GET_METHOD(b, rlshift), "<<");
}

Value valueRightShift(Value a, Value b) {
    return binaryMethod(a, b, GET_METHOD(a, rshift), GET_METHOD(b, rrshift), ">>");
}

Value valueContains(Value a, Value b) {
    BinaryMethod method = GET_METHOD(b, contains);
    if (method == NULL)
        return createException(VAL_TYPE_ERROR, "argument of type '%s' is not iterable", getValueType(a));
    return method(b, a);
}

Value valueGetAttribute(Value obj, ObjString *name) {
    Value (*method)(Value, ObjString*) = GET_METHOD(obj, getattr);
    if (method == NULL)
        reportRuntimeError("no method %s", name->chars);
    return method(obj, name);
}

Value valueSetAttribute(Value obj, ObjString *name, Value value) {
    Value (*method)(Value, ObjString*, Value) = GET_METHOD(obj, setattr);
    if (method == NULL)
        return createException(VAL_ATTRIBUTE_ERROR, "'%s' object attributes are read-only", getValueType(obj));
    return method(obj, name, value);
}

Value valueDelAttribute(Value obj, ObjString *name) {
    Value (*method)(Value, ObjString*) = GET_METHOD(obj, delattr);
    if (method == NULL)
        return createException(VAL_ATTRIBUTE_ERROR, "'%s' object attributes are read-only", getValueType(obj));
    return method(obj, name);
}

Value valueGetItem(Value obj, Value key) {
    Value (*method)(Value, Value) = GET_METHOD(obj, getitem);
    if (method == NULL)
        return createException(VAL_TYPE_ERROR, "'%s' object is not subscriptable", getValueType(obj));
    return method(obj, key);
}

Value valueSetItem(Value obj, Value key, Value value) {
    Value (*method)(Value, Value, Value) = GET_METHOD(obj, setitem);
    if (method == NULL)
        return createException(VAL_TYPE_ERROR, "'%s' object does not support item assignment", getValueType(obj));
    return method(obj, key, value);
}

Value valueDelItem(Value obj, Value key) {
    Value (*method)(Value, Value) = GET_METHOD(obj, delitem);
    if (method == NULL)
        return createException(VAL_TYPE_ERROR, "'%s' object does not support item deletion", getValueType(obj));
    return method(obj, key);
}

Value valueInit(Value callee, int argc, Value *argv) {
    Value (*method)(Value, int, Value*) = GET_METHOD(callee, init);
    return method(callee, argc, argv);
}

Value valueCall(Value callee, int argc, int kwargc, Value *argv) {
    Value (*method)(Value, int, int, Value*) = GET_METHOD(callee, call);
    if (method == NULL)
        return createException(VAL_TYPE_ERROR, "'%s' object is not callable", getValueType(callee));
    return method(callee, argc, kwargc, argv);
}

Value valueClass(Value value) {
    Value (*method)(Value) = GET_METHOD(value, class);
    if (method == NULL)
        return UNDEFINED_VAL;
    return method(value);
}

Value valueIter(Value value) {
    Value (*method)(Value) = GET_METHOD(value, iter);
    if (method == NULL)
        return createException(VAL_TYPE_ERROR, "'%s' object is not iterable", getValueType(value));
    return method(value);
}

Value valueNext(Value value) {
    Value (*method)(Value) = GET_METHOD(value, next);
    if (method == NULL)
        return createException(VAL_TYPE_ERROR, "'%s' object is not an iterator", getValueType(value));
    return method(value);
}

uint64_t valueHash(Value value) {
    uint64_t (*method)(Value) = GET_METHOD(value, hash);
    if (method == NULL)
        reportRuntimeError("unhashable type: '%s'", getValueType(value));
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
    long long (*method)(Value) = GET_METHOD(value, len);
    if (method == NULL)
        reportRuntimeError("object of type '%s' has not len()", getValueType(value));
    return method(value);
}

bool valueToBool(Value value) {
    bool (*method)(Value) = GET_METHOD(value, toBool);
    if (method == NULL)
        reportRuntimeError("object of type '%s' can not be converted to bool", getValueType(value));
    return method(value);
}

long long valueToInt(Value value) {
    long long (*method)(Value) = GET_METHOD(value, toInt);
    if (method == NULL)
        reportRuntimeError("object of type '%s' can not be converted to int", getValueType(value));
    return method(value);
}

double valueToFloat(Value value) {
    double (*method)(Value) = GET_METHOD(value, toFloat);
    if (method == NULL)
        reportRuntimeError("object of type '%s' can not be converted to float", getValueType(value));
    return method(value);
}

int valueWrite(Value value, char *buffer, size_t size) {
    int (*str)(Value, char*, size_t) = GET_METHOD(value, str);
    if (str == NULL)
        return Object_ToStr(value, NULL, 0);
    return str(value, buffer, size);
}

int valuePrint(Value value) {
    int (*str)(Value, char*, size_t) = GET_METHOD(value, str);
    if (str == NULL)
        return Object_ToStr(value, NULL, 0);
    return str(value, NULL, 0);
}

int valueReprWrite(Value value, char *buffer, size_t size) {
    int (*repr)(Value, char*, size_t) = GET_METHOD(value, repr);
    if (repr == NULL)
        return Object_ToStr(value, buffer, size);
    return repr(value, buffer, size);
}

int valueRepr(Value value) {
    int (*repr)(Value, char*, size_t) = GET_METHOD(value, repr);
    if (repr == NULL)
        return Object_ToStr(value, NULL, 0);
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

static Value getSuperClass(Value obj) {
    if (IS_CLASS(obj))
        return AS_CLASS(obj)->super;
    if (IS_NATIVE_CLASS(obj)) {
        ValueType superType = AS_NATIVE_CLASS(obj)->super;
        Value dummy = (Value){.type=superType};
        return valueClass(dummy);
    }
    if (IS_UNDEFINED(obj))
        return NONE_VAL;
    reportRuntimeError("you are not supposed to be here :<");
}

static bool r(Value obj, Value class) {
    if (valueToBool(valueEqual(obj, class)))
        return true;
    Value super = getSuperClass(obj);
    if (IS_NONE(super) || IS_UNDEFINED(super))
        return false;
    return r(super, class);
}

bool isInstance(Value obj, Value class) {
    return r(valueClass(obj), class);
}