#include "object_function.h"
#include "object_string.h"
#include "value_methods.h"
#include "memory.h"
#include "vm.h"

ObjUpvalue *createUpvalue(Value *slot) {
    ObjUpvalue *upvalue = (ObjUpvalue*)allocateObject(sizeof(ObjUpvalue), VAL_UPVALUE);
    upvalue->closed = NONE_VAL;
    upvalue->location = slot;
    upvalue->next = NULL;
    return upvalue;
}

ObjFunction* createFunction() {
    ObjFunction *function = (ObjFunction*)allocateObject(sizeof(ObjFunction), VAL_FUNCTION);
    function->arity = 0;
    function->defaultStart = -1;
    function->extraArgs = -1;
    function->extraKwargs = -1;
    function->upvalueCount = 0;
    function->name = NULL;
    initCodeVec(&function->code);
    return function;
}

ObjClosure* createClosure(ObjFunction *function) {
    ObjUpvalue **upvalues = ALLOCATE(ObjUpvalue*, function->upvalueCount); 
    for (int i = 0; i < function->upvalueCount; i++)
        upvalues[i] = NULL;

    ObjClosure *closure = (ObjClosure*)allocateObject(sizeof(ObjClosure), VAL_CLOSURE);
    closure->function = function;
    closure->upvalues = upvalues;
    closure->upvalueCount = function->upvalueCount;
    return closure;
}

ObjNative* createNative(NativeFn function, const char *name) {
    ObjNative *native = (ObjNative*)allocateObject(sizeof(ObjNative), VAL_NATIVE);
    native->function = function;
    native->name = name;
    return native;
}

int Function_ToStr(Value value, char *buffer, size_t size) {
    return writeToBuffer(buffer, size, "<function %s>", AS_FUNCTION(value)->name->chars);
}

Value Closure_Call(Value callee, int argc, int kwargc, Value *argv) {
    ObjClosure *closure = AS_CLOSURE(callee);
    call(closure, argc, kwargc, false);
}

int Closure_ToStr(Value value, char *buffer, size_t size) {
    writeToBuffer(buffer, size, "<function %s at %p>", AS_CLOSURE(value)->function->name->chars, valueId(value));
}

Value Native_Call(Value callee, int argc, int kwargc, Value *argv) {
    ObjNative *native = AS_NATIVE(callee);
    Value res = native->function(argc, kwargc);
    vm.top -= argc + 2 * kwargc + 1;
    push(res);
}

int Native_ToStr(Value value, char *buffer, size_t size) {
    writeToBuffer(buffer, size, "<built-in function %s>", AS_NATIVE(value)->name);
}