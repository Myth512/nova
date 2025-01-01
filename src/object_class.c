#include "object_class.h"
#include "object_string.h"
#include "value_int.h"
#include "value_methods.h"
#include "vm.h"

ObjClass *createClass(ObjString *name) {
    ObjClass *class = (ObjClass*)allocateObject(sizeof(ObjClass), VAL_CLASS);
    class->name = name;
    initTable(&class->methods);
    return class;
}

ObjNativeClass *createNativeClass(ObjString *name, ValueType type) {
    ObjNativeClass *class = (ObjNativeClass*)allocateObject(sizeof(ObjNativeClass), VAL_NATIVE_CLASS);
    class->name = name;
    class->type = type;
    return class;
}

ObjMethod *createMethod(Value reciever, ObjClosure *function) {
    ObjMethod *method = (ObjMethod*)allocateObject(sizeof(ObjMethod), VAL_METHOD);
    method->reciever = reciever;
    method->method = function;
    return method;
}

ObjNativeMethod *createNativeMethod(Value reciever, NativeFn function, const char *name) {
    ObjNativeMethod *native = (ObjNativeMethod*)allocateObject(sizeof(ObjNativeMethod), VAL_NATIVE_METHOD);
    native->reciever = reciever;
    native->name = name;
    native->method = function;
    return native;
}

int Class_ToStr(Value value, char *buffer, size_t size) {
    return writeToBuffer(buffer, size, "<class '%s'>", AS_CLASS(value)->name->chars);
}

int NativeClass_ToStr(Value value, char *buffer, size_t size) {
    return writeToBuffer(buffer, size, "<class '%s'>", AS_NATIVE_CLASS(value)->name->chars);
}

Value NativeClass_Call(Value value, int argc, Value *argv) {
    Value dummy = (Value){.type=AS_NATIVE_CLASS(value)->type};
    argv -= argc;
    return valueInit(dummy, argc, argv);
}