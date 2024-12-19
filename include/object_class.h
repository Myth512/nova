#ifndef OBJECT_CLASS_H
#define OBJECT_CLASS_H

#include "object.h"

#define IS_CLASS(value)         isObjType(value, OBJ_CLASS)
#define IS_INSTANCE(value)      isObjType(value, OBJ_INSTANCE)
#define IS_METHOD(value)        isObjType(value, OBJ_METHOD)
#define IS_NATIVE_METHOD(value) isObjType(value, OBJ_NATIVE_METHOD)

#define AS_CLASS(value)         ((ObjClass*)AS_OBJ(value))
#define AS_INSTANCE(value)      ((ObjInstance*)AS_OBJ(value))
#define AS_METHOD(value)        ((ObjMethod*)AS_OBJ(value))
#define AS_NATIVE_METHOD(value) ((ObjNativeMethod*)AS_OBJ(value))

typedef struct {
    Obj obj;
    ObjString *name;
    Table methods;
} ObjClass;

typedef struct {
    Obj obj;
    ObjClass *class;
    Table fields;
} ObjInstance;

typedef struct {
    Obj obj;
    Value reciever;
    ObjClosure *method;
} ObjMethod;

typedef struct {
    Obj obj;
    Value reciever;
    char *name;
    NativeFn method;
} ObjNativeMethod;

ObjClass *createClass(ObjString *name);

ObjInstance *createInstance(ObjClass *class);

ObjMethod *createMethod(Value reciever, ObjClosure *method);

ObjNativeMethod *createNativeMethod(Value reciever, NativeFn function, const char *name);

bool instanceEqual(Value a, Value b);

bool instanceNotEqual(Value a, Value b);

bool instanceGreater(Value a, Value b);

bool instanceGreaterEqual(Value a, Value b);

bool instanceLess(Value a, Value b);

bool instanceLessEqual(Value a, Value b);

Value instanceAdd(Value a, Value b);

Value instanceSubtract(Value a, Value b);

Value instanceMultiply(Value a, Value b);

Value instanceDivide(Value a, Value b);

Value instanceModulo(Value a, Value b);

Value instancePower(Value a, Value b);

Value instanceNegate(Value a);

Value instanceIncrement(Value a);

Value instanceDecrement(Value a);

OptValue instanceGetField(Value obj, ObjString *name);

void instanceSetField(Value obj, ObjString *name, Value value);

Value instanceGetAt(Value obj, Value key);

void instanceSetAt(Value obj, Value key, Value value);

int instanceLen(Value value);

bool instanceToBool(Value value);

int instanceToInt(Value value);

double instanceToFloat(Value value);

void instancePrint(Value instance);

#endif