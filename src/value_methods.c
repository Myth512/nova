#include "value_methods.h"
#include "value_none.h"
#include "value_int.h"
#include "value_float.h"
#include "object_string.h"
#include "vm.h"

#define GET_METHOD(value, name) MethodTable[(value).type].name

ValueMethods MethodTable[] = {
    [VAL_NONE]   = {None_Equal,   None_NotEqual,   NULL,           NULL,                NULL,        NULL,             NULL,       NULL,           NULL,             NULL,              NULL,              NULL,         NULL,        NULL,           NULL,           NULL,    NULL,    NULL,   NULL,       NULL,          NULL,           None_Hash,   NULL,       None_ToBool,   None_ToInt,   None_ToFloat,   None_ToStr,   None_ToStr},
    [VAL_BOOL]   = {Int_Equal,    Int_NotEqual,    Int_Greater,    Int_GreaterEqual,    Int_Less,    Int_LessEqual,    Int_Add,    Int_Subtract,   Int_Multiply,     Int_TrueDivide,    Int_FloorDivide,   Int_Modulo,   Int_Power,   Int_Positive,   Int_Negative,   Int_And, Int_Xor, Int_Or, Int_Invert, Int_LeftShift, Int_RightShift, Int_Hash,    NULL,       Int_ToBool,    Int_ToInt,    Int_ToFloat,    Bool_ToStr,   Bool_ToStr},
    [VAL_INT]    = {Int_Equal,    Int_NotEqual,    Int_Greater,    Int_GreaterEqual,    Int_Less,    Int_LessEqual,    Int_Add,    Int_Subtract,   Int_Multiply,     Int_TrueDivide,    Int_FloorDivide,   Int_Modulo,   Int_Power,   Int_Positive,   Int_Negative,   Int_And, Int_Xor, Int_Or, Int_Invert, Int_LeftShift, Int_RightShift, Int_Hash,    NULL,       Int_ToBool,    Int_ToInt,    Int_ToFloat,    Int_ToStr,    Int_ToStr},
    [VAL_FLOAT]  = {Float_Equal,  Float_NotEqual,  Float_Greater,  Float_GreaterEqual,  Float_Less,  Float_LessEqual,  Float_Add,  Float_Subtract, Float_Multiply,   Float_TrueDivide,  Float_FloorDivide, Float_Modulo, Float_Power, Float_Positive, Float_Negative, NULL,    NULL,    NULL,   NULL,       NULL,          NULL,           Float_Hash,  NULL,       Float_ToBool,  Float_ToInt,  Float_ToFloat,  Float_ToStr,  Float_ToStr},
    [VAL_STRING] = {String_Equal, String_NotEqual, String_Greater, String_GreaterEqual, String_Less, String_LessEqual, String_Add, NULL,           String_Multiply,  NULL,              NULL,              NULL,         NULL,        NULL,           NULL,           NULL,    NULL,    NULL,   NULL,       NULL,          NULL,           String_Hash, String_Len, String_ToBool, String_ToInt, String_ToFloat, String_ToStr, String_ToStr} 
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