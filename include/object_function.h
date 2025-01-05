#ifndef OBJECT_FUNCTION_H
#define OBJECT_FUNCTION_H

#include "object.h"
#include "object_tuple.h"

#define CLOSURE_VAL(closure)     ((Value){.type=VAL_CLOSURE, .as.object=(Obj*)closure})

#define IS_CLOSURE(value)       isObjType(value, OBJ_CLOSURE)
#define IS_FUNCTION(value)      isObjType(value, OBJ_FUNCTION)
#define IS_NATIVE(value)        isObjType(value, OBJ_NATIVE)

#define AS_CLOSURE(value)       ((ObjClosure*)value.as.object)
#define AS_FUNCTION(value)      ((ObjFunction*)value.as.object)
#define AS_NATIVE(value)        ((ObjNative*)value.as.object)
#define AS_UPVALUE(value)       ((ObjUpvalue*)value.as.object)

typedef struct ObjUpvalue {
    Obj obj;
    Value *location;
    Value closed;
    struct ObjUpvalue *next;
} ObjUpvalue;

typedef struct {
    Obj obj;
    int arity;
    int defaultStart;
    ObjTuple *defaults;
    ObjTuple *localNames;
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

typedef struct {
    Obj obj;
    char *name; 
    NativeFn function;
} ObjNative;

#define FUNCTION_METHODS (ValueMethods){ \
    .str = Function_ToStr,               \
    .repr = Function_ToStr               \
}

#define CLOSURE_METHODS (ValueMethods){ \
    .call = Closure_Call,               \
    .str = Closure_ToStr,               \
    .repr = Closure_ToStr               \
} 

#define NATIVE_METHODS (ValueMethods){ \
    .call = Native_Call,               \
    .str =  Native_ToStr,              \
    .repr = Native_ToStr               \
}

ObjUpvalue *createUpvalue(Value *slot);

ObjFunction* createFunction();

ObjClosure* createClosure(ObjFunction *function);

ObjNative* createNative(NativeFn function, const char *name);

int Function_ToStr(Value value, char *buffer, size_t size);

Value Closure_Call(Value callee, int argc, int kwargc, Value *argv);

int Closure_ToStr(Value value, char *buffer, size_t size);

Value Native_Call(Value callee, int argc, int kwargc, Value *argv);

int Native_ToStr(Value value, char *buffer, size_t size);

#endif