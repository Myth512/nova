#include "object_instance.h"

ObjInstance *createInstance(ObjClass *class) {
    ObjInstance *instance = (ObjInstance*)allocateObject(sizeof(ObjInstance), VAL_INSTANCE);
    instance->class = class;
    instance->isInitiazed = false;
    initTable(&instance->fields);
    return instance;
}

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

Value Instance_DelAttr(Value a, ObjString *name);

Value Instance_GetItem(Value obj, Value key);

Value Instance_SetItem(Value obj, Value key, Value value);

Value Instance_DelItem(Value obj, Value key);

uint64_t Instance_Hash(Value value);

long long Instance_Len(Value value);

bool Instance_ToBool(Value value);

int Instance_ToInt(Value value);

double Instance_ToFloat(Value value);

int Instance_ToStr(Value, char *buffer, size_t size);

int Instance_ToRepr(Value, char *buffer, size_t size);