#ifndef OBJECT_FUNCTION_H
#define OBJECT_FUNCTION_H

#include "object.h"
#include "object_array.h"

#define IS_CLOSURE(value)       isObjType(value, OBJ_CLOSURE)
#define IS_FUNCTION(value)      isObjType(value, OBJ_FUNCTION)
#define IS_NATIVE(value)        isObjType(value, OBJ_NATIVE)

#define AS_CLOSURE(value)       ((ObjClosure*)AS_OBJ(value))
#define AS_FUNCTION(value)      ((ObjFunction*)AS_OBJ(value))
#define AS_NATIVE(value)        ((ObjNative*)AS_OBJ(value))
#define AS_UPVALUE(value)       ((ObjUpvalue*)AS_OBJ(value))

typedef struct ObjUpvalue {
    Obj obj;
    Value *location;
    Value closed;
    struct ObjUpvalue *next;
} ObjUpvalue;

typedef struct {
    Obj obj;
    int minArity;
    int maxArity;
    ObjArray *defaults;
    ObjArray *localNames;
    int upvalueCount;
    CodeVec code;
    ObjString *name;
} ObjFunction;

typedef struct {
    Obj obj;
    ObjFunction *function;
    ObjUpvalue **upvalues;
    int upvalueCount;
} ObjClosure;

typedef Value (*NativeFn)(int argc, Value *argv);

typedef struct {
    Obj obj;
    char *name; 
    NativeFn function;
} ObjNative;

ObjUpvalue *createUpvalue(Value *slot);

ObjFunction* createFunction();

ObjClosure* createClosure(ObjFunction *function);

ObjNative* createNative(NativeFn function, const char *name);

#endif