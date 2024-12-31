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
    .sub = Int_Subtract,              \
    .mul = Int_Multiply,              \
    .truediv = Int_TrueDivide,        \
    .floordiv = Int_FloorDivide,      \
    .mod = Int_Modulo,                \
    .pow = Int_Power,                 \
    .pos = Int_Positive,              \
    .neg = Int_Negative,              \
    .and = Int_And,                   \
    .xor = Int_Xor,                   \
    .or = Int_Or,                     \
    .invert = Int_Invert,             \
    .lshift = Int_LeftShift,          \
    .rshift = Int_RightShift,         \
    .getattr = Int_GetAttr,           \
    .hash = Int_Hash,                 \
    .toBool = Int_ToBool,             \
    .toInt = Int_ToInt,               \
    .toFloat = Int_ToFloat,           \
    .str = Bool_ToStr,                \
    .repr = Bool_ToStr                \
}

#define INT_METHODS (ValueMethods) { \
    .eq = Int_Equal,                 \
    .ne = Int_NotEqual,              \
    .gt = Int_Greater,               \
    .ge = Int_GreaterEqual,          \
    .lt = Int_Less,                  \
    .le = Int_LessEqual,             \
    .add = Int_Add,                  \
    .sub = Int_Subtract,             \
    .mul = Int_Multiply,             \
    .truediv = Int_TrueDivide,       \
    .floordiv = Int_FloorDivide,     \
    .mod = Int_Modulo,               \
    .pow = Int_Power,                \
    .pos = Int_Positive,             \
    .neg = Int_Negative,             \
    .and = Int_And,                  \
    .xor = Int_Xor,                  \
    .or = Int_Or,                    \
    .invert = Int_Invert,            \
    .lshift = Int_LeftShift,         \
    .rshift = Int_RightShift,        \
    .getattr = Int_GetAttr,          \
    .init = Int_Init,                \
    .hash = Int_Hash,                \
    .toBool = Int_ToBool,            \
    .toInt = Int_ToInt,              \
    .toFloat = Int_ToFloat,          \
    .str = Int_ToStr,                \
    .repr = Int_ToStr                \
}

Value Int_Equal(Value a, Value b);

Value Int_NotEqual(Value a, Value b);

Value Int_Greater(Value a, Value b);

Value Int_GreaterEqual(Value a, Value b);

Value Int_Less(Value a, Value b);

Value Int_LessEqual(Value a, Value b);

Value Int_Add(Value a, Value b);

Value Int_Subtract(Value a, Value b);

Value Int_Multiply(Value a, Value b);

Value Int_TrueDivide(Value a, Value b);

Value Int_FloorDivide(Value a, Value b);

Value Int_Modulo(Value a, Value b);

Value Int_Power(Value a, Value b);

Value Int_Positive(Value a);

Value Int_Negative(Value a);

Value Int_And(Value a, Value b); 

Value Int_Xor(Value a, Value b);

Value Int_Or(Value a, Value b);

Value Int_Invert(Value a);

Value Int_LeftShift(Value a, Value b);

Value Int_RightShift(Value a, Value b);

Value Int_GetAttr(Value obj, ObjString *name);

Value Int_Init(int argc, Value *argv);

uint64_t Int_Hash(Value value);

bool Int_ToBool(Value value);

long long Int_ToInt(Value value);

double Int_ToFloat(Value value);

int Int_ToStr(Value value, char *buffer, const size_t size);

int Bool_ToStr(Value value, char *buffer, const size_t size);

#endif