#include "object_function.h"
#include "memory.h"

ObjUpvalue *createUpvalue(Value *slot) {
    ObjUpvalue *upvalue = (ObjUpvalue*)allocateObject(sizeof(ObjUpvalue));
    upvalue->closed = NONE_VAL;
    upvalue->location = slot;
    upvalue->next = NULL;
    return upvalue;
}

ObjFunction* createFunction() {
    ObjFunction *function = (ObjFunction*)allocateObject(sizeof(ObjFunction));
    function->minArity = 0;
    function->maxArity = 0;
    function->upvalueCount = 0;
    function->name = NULL;
    initCodeVec(&function->code);
    return function;
}

ObjClosure* createClosure(ObjFunction *function) {
    ObjUpvalue **upvalues = ALLOCATE(ObjUpvalue*, function->upvalueCount); 
    for (int i = 0; i < function->upvalueCount; i++)
        upvalues[i] = NULL;

    ObjClosure *closure = (ObjClosure*)allocateObject(sizeof(ObjClosure));
    closure->function = function;
    closure->upvalues = upvalues;
    closure->upvalueCount = function->upvalueCount;
    return closure;
}

ObjNative* createNative(NativeFn function, const char *name) {
    ObjNative *native = (ObjNative*)allocateObject(sizeof(ObjNative));
    native->function = function;
    native->name = name;
    return native;
}