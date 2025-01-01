#include "object_class.h"
#include "object_string.h"
#include "value_int.h"
#include "value_methods.h"
#include "object_instance.h"
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

Value Class_Call(Value callee, int argc, Value *argv) {
    ObjClass *class = AS_CLASS(callee);
    insert(argc, OBJ_VAL(createInstance(class)));
    Value initializer;
    if (tableGet(&class->methods, vm.magicStrings.init, &initializer)) {
        call(AS_CLOSURE(initializer), argc + 1, true);
    } else if (argc != 0) {
        reportRuntimeError("Expect 0 arguments but got %d", argc);
    }
}

int NativeClass_ToStr(Value value, char *buffer, size_t size) {
    return writeToBuffer(buffer, size, "<class '%s'>", AS_NATIVE_CLASS(value)->name->chars);
}

Value NativeClass_Call(Value callee, int argc, Value *argv) {
    Value dummy = (Value){.type=AS_NATIVE_CLASS(callee)->type};
    Value res = valueInit(dummy, argc, argv - argc);
    vm.top -= argc + 1;
    push(res);
}

Value Method_Call(Value callee, int argc, Value *argv) {
    ObjMethod *method = AS_METHOD(callee);
    insert(argc, method->reciever);
    call(method->method, argc + 1, true);
}

int Method_ToStr(Value value, char *buffer, size_t size) {
    return writeToBuffer(buffer, size, "<bound method %s>", AS_METHOD(value)->method->function->name->chars);
}

int NativeMethod_ToStr(Value value, char *buffer, size_t size) {
    return writeToBuffer(buffer, size, "<method-wrapper '%s'>", AS_NATIVE_METHOD(value)->name);
}

Value NativeMethod_Call(Value callee, int argc, Value *argv) {
    ObjNativeMethod *method = AS_NATIVE_METHOD(callee);
    insert(argc, method->reciever);
    NativeFn native = method->method;
    Value result = native(argc, vm.top - argc - 1);
    vm.top -= argc + 1;
    push(result);
}