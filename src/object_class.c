#include "object_class.h"
#include "object_string.h"
#include "value_int.h"
#include "value_methods.h"
#include "object_instance.h"
#include "object_exception.h"
#include "vm.h"

ObjClass *createClass(ObjString *name, Value super) {
    ObjClass *class = (ObjClass*)allocateObject(sizeof(ObjClass), VAL_CLASS);
    class->name = name;
    class->super = super;
    initNameTable(&class->methods);
    return class;
}

ObjNativeClass *createNativeClass(ObjString *name, ValueType type, ValueType super) {
    ObjNativeClass *class = (ObjNativeClass*)allocateObject(sizeof(ObjNativeClass), VAL_NATIVE_CLASS);
    class->name = name;
    class->type = type;
    class->super = super;
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

Value Class_Call(Value callee, int argc, int kwargc, Value *argv) {
    ObjClass *class = AS_CLASS(callee);
    insert(argc, OBJ_VAL(createInstance(class)));
    Value initializer;
    if (nameTableGet(&class->methods, vm.magicStrings.init, &initializer)) {
        call(AS_CLOSURE(initializer), argc + 1, kwargc, true);
    } else if (argc != 0) {
        return createException(VAL_TYPE_ERROR, "%s() takes no arguments", class->name->chars);
    }
}

Value Class_GetAttr(Value obj, ObjString *name) {
    ObjClass *class = AS_CLASS(obj);
    Value value;
    if (nameTableGet(&class->methods, name, &value))
        return value;
    if (IS_NONE(class->super))
        return UNDEFINED_VAL;
    return valueGetAttribute(class->super, name);
}

Value Class_Class(Value value) {
    return OBJ_VAL(vm.types.type);
}

int NativeClass_ToStr(Value value, char *buffer, size_t size) {
    return writeToBuffer(buffer, size, "<class '%s'>", AS_NATIVE_CLASS(value)->name->chars);
}

Value NativeClass_Call(Value callee, int argc, int kwargc, Value *argv) {
    Value dummy = (Value){.type=AS_NATIVE_CLASS(callee)->type};
    Value res = valueInit(dummy, argc, argv - argc);
    vm.top -= argc + 1;
    push(res);
    raiseIfException();
}

Value Method_Call(Value callee, int argc,  int kwargc, Value *argv) {
    ObjMethod *method = AS_METHOD(callee);
    insert(argc + 2*kwargc, method->reciever);
    call(method->method, argc + 1, kwargc, true);
}

int Method_ToStr(Value value, char *buffer, size_t size) {
    return writeToBuffer(buffer, size, "<bound method %s>", AS_METHOD(value)->method->function->name->chars);
}

int NativeMethod_ToStr(Value value, char *buffer, size_t size) {
    return writeToBuffer(buffer, size, "<method-wrapper '%s'>", AS_NATIVE_METHOD(value)->name);
}

Value NativeMethod_Call(Value callee, int argc, int kwargc, Value *argv) {
    ObjNativeMethod *method = AS_NATIVE_METHOD(callee);
    insert(argc + 2*kwargc, method->reciever);
    NativeFn native = method->method;
    Value result = native(argc + 1, kwargc); 
    vm.top -= argc + 1;
    push(result);
    raiseIfException();
}