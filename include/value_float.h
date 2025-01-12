
#ifndef VALUE_FLOAT_H
#define VALUE_FLOAT_H

#include "value.h"

#define FLOAT_VAL(value)    ((Value){VAL_FLOAT, {.floating=value}})

#define IS_FLOAT(value)     ((value).type == VAL_FLOAT)

#define AS_FLOAT(value)     ((value).as.floating)

#define FLOAT_METHODS (ValueMethods) {   \
    .eq = Float_Equal,                   \
    .ne = Float_NotEqual,                \
    .gt = Float_Greater,                 \
    .ge = Float_GreaterEqual,            \
    .lt = Float_Less,                    \
    .le = Float_LessEqual,               \
    .add = Float_Add,                    \
    .radd = Float_Add,                   \
    .sub = Float_Subtract,               \
    .rsub = Float_RightSubtract,         \
    .mul = Float_Multiply,               \
    .rmul = Float_Multiply,              \
    .truediv = Float_TrueDivide,         \
    .rtruediv = Float_RightTrueDivide,   \
    .floordiv = Float_FloorDivide,       \
    .rfloordiv = Float_RightFloorDivide, \
    .mod = Float_Modulo,                 \
    .rmod = Float_RightModulo,           \
    .pow = Float_Power,                  \
    .rpow = Float_RightPower,            \
    .pos = Float_Positive,               \
    .neg = Float_Negative,               \
    .init = Float_Init,                  \
    .class = Float_Class,                \
    .getattr = Float_GetAttribute,       \
    .hash = Float_Hash,                  \
    .toBool = Float_ToBool,              \
    .toInt = Float_ToInt,                \
    .toFloat = Float_ToFloat,            \
    .str = Float_ToStr,                  \
    .repr = Float_ToStr                  \
}

Value Float_Equal(Value a, Value b);

Value Float_NotEqual(Value a, Value b);

Value Float_Greater(Value a, Value b);

Value Float_GreaterEqual(Value a, Value b);

Value Float_Less(Value a, Value b);

Value Float_LessEqual(Value a, Value b);

Value Float_Add(Value a, Value b);

Value Float_Subtract(Value a, Value b);

Value Float_RightSubtract(Value a, Value b);

Value Float_Multiply(Value a, Value b);

Value Float_TrueDivide(Value a, Value b);

Value Float_RightTrueDivide(Value a, Value b);

Value Float_FloorDivide(Value a, Value b);

Value Float_RightFloorDivide(Value a, Value b);

Value Float_Modulo(Value a, Value b);

Value Float_RightModulo(Value a, Value b);

Value Float_Power(Value a, Value b);

Value Float_RightPower(Value a, Value b);

Value Float_Positive(Value a);

Value Float_Negative(Value a);

Value Float_Class(Value value);

Value Float_Init(Value callee, int argc, Value *argv);

Value Float_GetAttribute(Value a, ObjString *name);

uint64_t Float_Hash(Value value);

bool Float_ToBool(Value value);

long long Float_ToInt(Value value);

double Float_ToFloat(Value value);

int Float_ToStr(Value value, char *buffer, const size_t size);

#endif