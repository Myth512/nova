#include "object_instance.h"
#include "value_methods.h"
#include "object_exception.h"
#include "object_string.h"
#include "vm.h"

ObjInstance *createInstance(ObjClass *class) {
    ObjInstance *instance = (ObjInstance*)allocateObject(sizeof(ObjInstance), VAL_INSTANCE);
    instance->class = class;
    instance->isInitiazed = false;
    initNameTable(&instance->attributes);
    return instance;
}

Value Instance_Equal(Value a, Value b) {
    return NOT_IMPLEMENTED_VAL;
}

Value Instance_NotEqual(Value a, Value b) {

}

Value Instance_Greater(Value a, Value b) {

}

Value Instance_GreaterEqual(Value a, Value b) {

} 

Value Instance_Less(Value a, Value b) {

}

Value Instance_LessEqual(Value a, Value b) {

}

Value Instance_Add(Value a, Value b) {

}

Value Instance_Subtract(Value a, Value b) {

}

Value Instance_Multiply(Value a, Value b) {

}

Value Instance_TrueDivide(Value a, Value b) {

}

Value Instance_FloorDivide(Value a, Value b) {

}

Value Instance_Modulo(Value a, Value b) {

}

Value Instance_Power(Value a, Value b) {

}

Value Instance_Positive(Value a) {

}

Value Instance_Negative(Value a) {

}

Value Instance_And(Value a, Value b) {

}

Value Instance_Xor(Value a, Value b) {

}

Value Instance_Or(Value a, Value b) {

}

Value Instance_Invert(Value a) {

}

Value Instance_LeftShift(Value a, Value b) {

}

Value Instance_RightShift(Value a, Value b) {

}

Value Instance_Class(Value value) {
    return OBJ_VAL(AS_INSTANCE(value)->class);
}

Value Instance_GetAttr(Value obj, ObjString *name) {
    Value value;
    ObjInstance *instance = AS_INSTANCE(obj);

    if (nameTableGet(&instance->attributes, name, &value))
        return value;

    value = Class_GetAttr(OBJ_VAL(instance->class), name);

    if (IS_UNDEFINED(value))
        return createException(VAL_ATTRIBUTE_ERROR, "'%s' object has no attribute '%s'", AS_INSTANCE(obj)->class->name->chars, name->chars);

    return OBJ_VAL(createMethod(obj, AS_CLOSURE(value)));
}

Value Instance_SetAttr(Value obj, ObjString *name, Value value) {
    nameTableSet(&AS_INSTANCE(obj)->attributes, name, value);
}

Value Instance_DelAttr(Value obj, ObjString *name) {
    bool res = nameTableDelete(&AS_INSTANCE(obj)->attributes, name);
    if (!res)
        return createException(VAL_ATTRIBUTE_ERROR, "'%s' object has no attribute '%s'", AS_INSTANCE(obj)->class->name->chars, name->chars);
    return NONE_VAL;
}

Value Instance_GetItem(Value obj, Value key) {

}

Value Instance_SetItem(Value obj, Value key, Value value) {

}

Value Instance_DelItem(Value obj, Value key) {

}

uint64_t Instance_Hash(Value value) {

}

long long Instance_Len(Value value) {

}

bool Instance_ToBool(Value value) {

}

long long Instance_ToInt(Value value) {

}

double Instance_ToFloat(Value value) {

}

int Instance_ToStr(Value value, char *buffer, size_t size) {
    writeToBuffer(buffer, size, "%s object at %p", AS_INSTANCE(value)->class->name->chars, valueId(value));
}

int Instance_ToRepr(Value value, char *buffer, size_t size) {
    writeToBuffer(buffer, size, "%s object at %p", AS_INSTANCE(value)->class->name->chars, valueId(value));
}