#include <string.h>

#ifdef DEBUG_LOG_GC
    #include <stdio.h>
#endif

#include "object.h"
#include "vm.h"
#include "memory.h"
#include "value.h"
#include "table.h"
#include "common.h"


Obj* allocateObject(size_t size, ObjType type) {
    Obj *object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;
    object->next = vm.objects;
    object->isMarked = false;
    vm.objects = object;
    #ifdef DEBUG_LOG_GC
        printf("allocate %p, size %zu bytes, %s\n", object, size, decodeObjType(OBJ_VAL(object)));
    #endif
    return object;
}

ObjUpvalue *createUpvalue(Value *slot) {
    ObjUpvalue *upvalue = (ObjUpvalue*)allocateObject(sizeof(ObjUpvalue), OBJ_UPVALUE);
    upvalue->closed = NIL_VAL;
    upvalue->location = slot;
    upvalue->next = NULL;
    return upvalue;
}

ObjFunction* createFunction() {
    ObjFunction *function = (ObjFunction*)allocateObject(sizeof(ObjFunction), OBJ_FUNCTION);
    function->arity = 0;
    function->upvalueCount = 0;
    function->name = NULL;
    initCodeVec(&function->code);
    return function;
}

ObjClosure* createClosure(ObjFunction *function) {
    ObjUpvalue **upvalues = ALLOCATE(ObjUpvalue*, function->upvalueCount); 
    for (int i = 0; i < function->upvalueCount; i++)
        upvalues[i] = NULL;

    ObjClosure *closure = (ObjClosure*)allocateObject(sizeof(ObjClosure), OBJ_CLOSURE);
    closure->function = function;
    closure->upvalues = upvalues;
    closure->upvalueCount = function->upvalueCount;
    return closure;
}

ObjNative* createNative(NativeFn function, const char *name) {
    ObjNative *native = (ObjNative*)allocateObject(sizeof(ObjNative), OBJ_NATIVE);
    native->function = function;
    native->name = name;
    return native;
}
