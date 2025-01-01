#ifndef OBJECT_INSTANCE_H
#define OBJECT_INSTANCE_H

#include "object.h"
#include "object_class.h"

#define IS_INSTANCE(value)      isObjType(value, OBJ_INSTANCE)

#define AS_INSTANCE(value)      ((ObjInstance*)value.as.object)

typedef struct {
    Obj obj;
    ObjClass *class;
    Table attributes;
    bool isInitiazed;
} ObjInstance;

#define INSTANCE_METHODS (ValueMethods) { \
    .eq = Instance_Equal,                 \
    .ne = Instance_NotEqual,              \
    .gt = Instance_Greater,               \
    .ge = Instance_GreaterEqual,          \
    .lt = Instance_Less,                  \
    .le = Instance_LessEqual,             \
    .add = Instance_Add,                  \
    .sub = Instance_Subtract,             \
    .mul = Instance_Multiply,             \
    .truediv = Instance_TrueDivide,       \
    .floordiv = Instance_FloorDivide,     \
    .mod = Instance_Modulo,               \
    .pow = Instance_Power,                \
    .pos = Instance_Positive,             \
    .neg = Instance_Negative,             \
    .and = Instance_And,                  \
    .xor = Instance_Xor,                  \
    .or = Instance_Or,                    \
    .invert = Instance_Invert,            \
    .lshift = Instance_LeftShift,         \
    .rshift = Instance_RightShift,        \
    .getattr = Instance_GetAttr,          \
    .setattr = Instance_SetAttr,          \
    .delattr = Instance_DelAttr,          \
    .getitem = Instance_GetItem,          \
    .setitem = Instance_SetItem,          \
    .delitem = Instance_DelItem,          \
    .hash = Instance_Hash,                \
    .len = Instance_Len,                  \
    .toBool = Instance_ToBool,            \
    .toInt = Instance_ToInt,              \
    .toFloat = Instance_ToFloat,          \
    .str = Instance_ToStr,                \
    .repr = Instance_ToRepr               \
}

ObjInstance *createInstance(ObjClass *class);

Value Instance_Equal(Value a, Value b);

Value Instance_NotEqual(Value a, Value b);

Value Instance_Greater(Value a, Value b);

Value Instance_GreaterEqual(Value a, Value b);

Value Instance_Less(Value a, Value b);

Value Instance_LessEqual(Value a, Value b);

Value Instance_Add(Value a, Value b);

Value Instance_Subtract(Value a, Value b);

Value Instance_Multiply(Value a, Value b);

Value Instance_TrueDivide(Value a, Value b);

Value Instance_FloorDivide(Value a, Value b);

Value Instance_Modulo(Value a, Value b);

Value Instance_Power(Value a, Value b);

Value Instance_Positive(Value a);

Value Instance_Negative(Value a);

Value Instance_And(Value a, Value b);

Value Instance_Xor(Value a, Value b);

Value Instance_Or(Value a, Value b);

Value Instance_Invert(Value a);

Value Instance_LeftShift(Value a, Value b);

Value Instance_RightShift(Value a, Value b);

Value Instance_GetAttr(Value obj, ObjString *name);

Value Instance_SetAttr(Value obj, ObjString *name, Value value);

Value Instance_DelAttr(Value obj, ObjString *name);

Value Instance_GetItem(Value obj, Value key);

Value Instance_SetItem(Value obj, Value key, Value value);

Value Instance_DelItem(Value obj, Value key);

uint64_t Instance_Hash(Value value);

long long Instance_Len(Value value);

bool Instance_ToBool(Value value);

long long Instance_ToInt(Value value);

double Instance_ToFloat(Value value);

int Instance_ToStr(Value, char *buffer, size_t size);

int Instance_ToRepr(Value, char *buffer, size_t size);

#endif