#include "object_class.h"
#include "vm.h"

ObjClass *createClass(ObjString *name) {
    ObjClass *class = (ObjClass*)allocateObject(sizeof(ObjClass), OBJ_CLASS);
    class->name = name;
    initTable(&class->methods);
    return class;
}

ObjInstance *createInstance(ObjClass *class) {
    ObjInstance *instance = (ObjInstance*)allocateObject(sizeof(ObjInstance), OBJ_INSTANCE);
    instance->class = class;
    initTable(&instance->fields);
    return instance;
}

ObjMethod *createMethod(Value reciever, ObjClosure *function) {
    ObjMethod *method = (ObjMethod*)allocateObject(sizeof(ObjMethod), OBJ_METHOD);
    method->reciever = reciever;
    method->method = function;
    return method;
}

ObjNativeMethod *createNativeMethod(Value reciever, NativeFn function, const char *name) {
    ObjNativeMethod *native = (ObjNativeMethod*)allocateObject(sizeof(ObjNativeMethod), OBJ_NATIVE_METHOD);
    native->reciever = reciever;
    native->name = name;
    native->method = function;
    return native;
}

// static bool callBinaryMethod(ObjString *universal, ObjString *left, ObjString *right) {
//     Value method;

//     if (left && getProperty(peek(1), left, &method)) {
//         callValue(method, 1);
//         return true;
//     }

//     if (universal && getProperty(peek(1), universal, &method)) {
//         callValue(method, 1);
//         return true;
//     }

//     if (right && getProperty(peek(0), right, &method)) {
//         swapValues(vm.top - 1, vm.top - 2);
//         callValue(method, 1);
//         return true;
//     }

//     if (universal && getProperty(peek(0), universal, &method)) {
//         swapValues(vm.top - 1, vm.top - 2);
//         callValue(method, 1);
//         return true;
//     }

//     return false;
// }

// static bool callUnaryMethod(ObjString *universal) {
//     Value method;

//     if (getProperty(peek(0), universal, &method)) {
//         callValue(method, 0);
//         return true;
//     }

//     return false;
// }

Value instanceEqual(Value a, Value b) {
    Value result;
    bool success = callNovaMethod1arg(a, vm.magicStrings.eq, b, &result);
    if (success) {
        return result;
    }
}

Value instanceNotEqual(Value a, Value b) {

}

Value instanceGreater(Value a, Value b) {

}

Value instanceGreaterEqual(Value a, Value b) {

}

Value instanceLess(Value a, Value b) {

}

Value instanceLessEqual(Value a, Value b) {

}

Value instanceNot(Value a) {

}

Value instanceAdd(Value a, Value b) {

}

Value instanceSubtract(Value a, Value b) {

}

Value instanceMultiply(Value a, Value b) {

}

Value instanceDivide(Value a, Value b) {

}

Value instanceModulo(Value a, Value b) {

}

Value instancePower(Value a, Value b) {

}

Value instanceNegate(Value a) {

}

Value instanceIncrement(Value a) {

}

Value instanceDecrement(Value a) {

}

void instancePrint(Value instance) {

}