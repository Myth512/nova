#include "object_class.h"
#include "object_string.h"
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

Value instanceEqual(Value a, Value b) {
    OptValue result = callNovaMethod1arg(a, vm.magicStrings.eq, b);
    if (result.hasValue)
        return result.value;
    return BOOL_VAL(&a == &b);
}

Value instanceNotEqual(Value a, Value b) {
    OptValue result = callNovaMethod1arg(a, vm.magicStrings.ne, b);
    if (result.hasValue)
        return result.value;
    return BOOL_VAL(&a != &b);
}

static Value unary(Value a, char *operator, ObjString *methodName) {
    OptValue result = callNovaMethod(a, methodName, 0);
    if (result.hasValue)
        return result.value;
    reportTypeError1op(operator, a);
}

static Value binary(Value a, Value b, char *operator, ObjString *methodName) {
    OptValue result = callNovaMethod1arg(a, methodName, b);
    if (result.hasValue)
        return result.value;
    reportTypeError(operator, a, b);
}

static bool isUnsupported(Value value) {
    if (!IS_STRING(value))
        return false;
    return stringEqual(vm.magicStrings.unsupported, AS_STRING(value));
}

static Value arithmetic(Value a, Value b, char *operator, ObjString *universal, ObjString *left, ObjString *right) {
    OptValue result = callNovaMethod1arg(a, left, b);
    if (result.hasValue && !isUnsupported(result.value))
        return result.value;
    
    result = callNovaMethod1arg(a, universal, b);
    if (result.hasValue && !isUnsupported(result.value))
        return result.value;

    result = callNovaMethod1arg(b, right, a);
    if (result.hasValue && !isUnsupported(result.value))
        return result.value;

    result = callNovaMethod1arg(b, universal, a);
    if (result.hasValue && !isUnsupported(result.value))
        return result.value;
    
    reportTypeError(operator, a, b);
}

Value instanceGreater(Value a, Value b) {
    return binary(a, b, ">", vm.magicStrings.gt);
}

Value instanceGreaterEqual(Value a, Value b) {
    return binary(a, b, ">=", vm.magicStrings.ge);
}

Value instanceLess(Value a, Value b) {
    return binary(a, b, "<", vm.magicStrings.lt);
}

Value instanceLessEqual(Value a, Value b) {
    return binary(a, b, "<=", vm.magicStrings.le);
}

Value instanceNot(Value a) {
    reportTypeError1op("not", a);
}

Value instanceAdd(Value a, Value b) {
    return arithmetic(a, b, "+", vm.magicStrings.add, vm.magicStrings.ladd, vm.magicStrings.radd);
}

Value instanceSubtract(Value a, Value b) {
    return arithmetic(a, b, "-", vm.magicStrings.sub, vm.magicStrings.lsub, vm.magicStrings.rsub);
}

Value instanceMultiply(Value a, Value b) {
    return arithmetic(a, b, "*", vm.magicStrings.mul, vm.magicStrings.lmul, vm.magicStrings.rmul);
}

Value instanceDivide(Value a, Value b) {
    return arithmetic(a, b, "/", vm.magicStrings.div, vm.magicStrings.ldiv, vm.magicStrings.rdiv);
}

Value instanceModulo(Value a, Value b) {
    return arithmetic(a, b, "%%", vm.magicStrings.mod, vm.magicStrings.lmod, vm.magicStrings.rmod);
}

Value instancePower(Value a, Value b) {
    return arithmetic(a, b, "^", vm.magicStrings.pow, vm.magicStrings.lpow, vm.magicStrings.rpow);
}

Value instanceNegate(Value a) {
    return unary(a, "-", vm.magicStrings.neg);
}

Value instanceIncrement(Value a) {
    return unary(a, "++", vm.magicStrings.inc);
}

Value instanceDecrement(Value a) {
    return unary(a, "--", vm.magicStrings.dec);
}

void instancePrint(Value instance) {
    OptValue result = callNovaMethod(instance, vm.magicStrings.str, 0);
    if (result.hasValue) {
        printValue(result.value);
    } else {
        printf("instance of %s", decodeValueType(instance));
    }
}