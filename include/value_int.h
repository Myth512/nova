#ifndef VALUE_INT_H
#define VALUE_INT_H

#include "value.h"

#define BOOL_VAL(value)     ((Value){VAL_BOOL, {.integer=value}})
#define INT_VAL(value)      ((Value){VAL_INT, {.integer=value}}) 

#define IS_BOOL(value)      ((value).type == VAL_BOOL)
#define IS_INT(value)       ((value).type == VAL_INT)

#define AS_BOOL(value)      ((bool)((value).as.integer))
#define AS_INT(value)       ((value).as.integer)

#define BOOL_METHODS (ValueMethods) { \
    .eq = Int_Equal,                  \
    .ne = Int_NotEqual,               \
    .gt = Int_Greater,                \
    .ge = Int_GreaterEqual,           \
    .lt = Int_Less,                   \
    .le = Int_LessEqual,              \
    .add = Int_Add,                   \
    .radd = Int_Add,                  \
    .sub = Int_Subtract,              \
    .rsub = Int_RightSubtract,        \
    .mul = Int_Multiply,              \
    .rmul = Int_Multiply,             \
    .truediv = Int_TrueDivide,        \
    .rtruediv = Int_RightTrueDivide,  \
    .floordiv = Int_FloorDivide,      \
    .rfloordiv = Int_RightFloorDivide,\
    .mod = Int_Modulo,                \
    .rmod = Int_RightModulo,          \
    .pow = Int_Power,                 \
    .rpow = Int_RightPower,           \
    .pos = Int_Positive,              \
    .neg = Int_Negative,              \
    .and = Int_And,                   \
    .xor = Int_Xor,                   \
    .or = Int_Or,                     \
    .invert = Int_Invert,             \
    .lshift = Int_LeftShift,          \
    .rlshift = Int_RightLeftShift,    \
    .rshift = Int_RightShift,         \
    .rrshift = Int_RightRightShift,   \
    .getattr = Int_GetAttr,           \
    .init = Bool_Init,                \
    .class = Bool_Class,              \
    .hash = Int_Hash,                 \
    .toBool = Int_ToBool,             \
    .toInt = Int_ToInt,               \
    .toFloat = Int_ToFloat,           \
    .str = Bool_ToStr,                \
    .repr = Bool_ToStr                \
}

#define INT_METHODS (ValueMethods) {  \
    .eq = Int_Equal,                  \
    .ne = Int_NotEqual,               \
    .gt = Int_Greater,                \
    .ge = Int_GreaterEqual,           \
    .lt = Int_Less,                   \
    .le = Int_LessEqual,              \
    .radd = Int_Add,                  \
    .sub = Int_Subtract,              \
    .rsub = Int_RightSubtract,        \
    .mul = Int_Multiply,              \
    .rmul = Int_Multiply,             \
    .truediv = Int_TrueDivide,        \
    .rtruediv = Int_RightTrueDivide,  \
    .floordiv = Int_FloorDivide,      \
    .rfloordiv = Int_RightFloorDivide,\
    .mod = Int_Modulo,                \
    .rmod = Int_RightModulo,          \
    .pow = Int_Power,                 \
    .rpow = Int_RightPower,           \
    .pos = Int_Positive,              \
    .neg = Int_Negative,              \
    .and = Int_And,                   \
    .xor = Int_Xor,                   \
    .or = Int_Or,                     \
    .invert = Int_Invert,             \
    .lshift = Int_LeftShift,          \
    .rlshift = Int_RightLeftShift,    \
    .rshift = Int_RightShift,         \
    .rrshift = Int_RightRightShift,   \
    .getattr = Int_GetAttr,           \
    .init = Int_Init,                 \
    .class = Int_Class,               \
    .hash = Int_Hash,                 \
    .toBool = Int_ToBool,             \
    .toInt = Int_ToInt,               \
    .toFloat = Int_ToFloat,           \
    .str = Int_ToStr,                 \
    .repr = Int_ToStr                 \
}

Value Int_Equal(Value a, Value b);

Value Int_NotEqual(Value a, Value b);

Value Int_Greater(Value a, Value b);

Value Int_GreaterEqual(Value a, Value b);

Value Int_Less(Value a, Value b);

Value Int_LessEqual(Value a, Value b);

Value Int_Add(Value a, Value b);

Value Int_Subtract(Value a, Value b);

Value Int_RightSubtract(Value a, Value b);

Value Int_Multiply(Value a, Value b);

Value Int_TrueDivide(Value a, Value b);

Value Int_RightTrueDivide(Value a, Value b);

Value Int_FloorDivide(Value a, Value b);

Value Int_RightFloorDivide(Value a, Value b);

Value Int_Modulo(Value a, Value b);

Value Int_RightModulo(Value a, Value b);

Value Int_Power(Value a, Value b);

Value Int_RightPower(Value a, Value b);

Value Int_Positive(Value a);

Value Int_Negative(Value a);

Value Int_And(Value a, Value b); 

Value Int_Xor(Value a, Value b);

Value Int_Or(Value a, Value b);

Value Int_Invert(Value a);

Value Int_LeftShift(Value a, Value b);

Value Int_RightLeftShift(Value a, Value b);

Value Int_RightShift(Value a, Value b);

Value Int_RightRightShift(Value a, Value b);

Value Int_GetAttr(Value obj, ObjString *name);

Value Int_Init(Value callee, int argc, Value *argv);

Value Bool_Init(Value callee, int argc, Value *argv);

Value Int_Class(Value value);

Value Bool_Class(Value value);

uint64_t Int_Hash(Value value);

bool Int_ToBool(Value value);

long long Int_ToInt(Value value);

double Int_ToFloat(Value value);

int Int_ToStr(Value value, char *buffer, const size_t size);

int Bool_ToStr(Value value, char *buffer, const size_t size);

#endif