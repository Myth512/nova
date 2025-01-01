#ifndef OBJECT_CLASS_H
#define OBJECT_CLASS_H

#include "object.h"
#include "object_function.h"

#define NATIVE_VAL(native)      ((Value){.type=VAL_NATIVE, .as.object=(Obj*)native})

#define IS_CLASS(value)         isObjType(value, OBJ_CLASS)
#define IS_METHOD(value)        isObjType(value, OBJ_METHOD)
#define IS_NATIVE_METHOD(value) isObjType(value, OBJ_NATIVE_METHOD)

#define AS_CLASS(value)         ((ObjClass*)value.as.object)
#define AS_NATIVE_CLASS(value)  ((ObjNativeClass*)value.as.object)
#define AS_METHOD(value)        ((ObjMethod*)value.as.object)
#define AS_NATIVE_METHOD(value) ((ObjNativeMethod*)value.as.object)

#define CLASS_METHODS (ValueMethods){ \
    .call = Class_Call,               \
    .str = Class_ToStr,               \
    .repr = Class_ToStr,              \
}

#define NATIVE_CLASS_METHODS (ValueMethods){ \
    .call = NativeClass_Call,                \
    .str = NativeClass_ToStr,                \
    .repr = NativeClass_ToStr,               \
}

#define MEHTOD_METHODS (ValueMethods) { \
    .str = Method_ToStr,                \
    .repr = Method_ToStr                \
}

#define NATIVE_METHOD_METHODS (ValueMethods){ \
    .call = NativeMethod_Call,                \
    .str = NativeMethod_ToStr,                \
    .repr = Native_ToStr,                     \
}

typedef struct {
    Obj obj;
    ObjString *name;
    Table methods;
} ObjClass;

typedef struct {
    Obj obj;
    ObjString *name;
    ValueType type;
} ObjNativeClass;

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

ObjNativeClass *createNativeClass(ObjString *name, ValueType type);

ObjMethod *createMethod(Value reciever, ObjClosure *method);

ObjNativeMethod *createNativeMethod(Value reciever, NativeFn function, const char *name);

Value Class_Call(Value callee, int argc, Value *argv);

int Class_ToStr(Value value, char *buffer, size_t size);

Value NativeClass_Call(Value value, int argc, Value *argv);

int NativeClass_ToStr(Value value, char *buffer, size_t size);

int Method_ToStr(Value value, char *buffer, size_t size);

Value NativeMethod_Call(Value callee, int argc, Value *argv);

int NativeMethod_ToStr(Value value, char *buffer, size_t size);

#endif