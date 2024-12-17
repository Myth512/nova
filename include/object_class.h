#ifndef OBJECT_CLASS_H
#define OBJECT_CLASS_H

#include "object.h"

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

#endif