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
    instance->isInitiazed = false;
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

int instanceWrite(Value instance, char *buffer, const size_t size) {
    if (AS_INSTANCE(instance)->isInitiazed) {
        OptValue result = callNovaMethod(instance, vm.magicStrings.str);
        if (result.hasValue)
            return valueWrite(result.value, buffer, size);
    }
    return writeToBuffer(buffer, size, "instance of %s", AS_INSTANCE(instance)->class->name->chars);
}

int instancePrint(Value instance) {
    return instanceWrite(instance, NULL, 0);
}

bool instanceEqual(Value a, Value b) {
    OptValue result = callNovaMethod1arg(a, vm.magicStrings.eq, b);
    if (result.hasValue) {
        if (IS_BOOL(result.value))
            return AS_BOOL(result.value);
        reportRuntimeError("Return type must be <type bool>");
    }
    return &a == &b;
}

bool instanceNotEqual(Value a, Value b) {
    OptValue result = callNovaMethod1arg(a, vm.magicStrings.ne, b);
    if (result.hasValue) {
        if (IS_BOOL(result.value))
            return AS_BOOL(result.value);
        reportRuntimeError("Return type must be <type bool>");
    }
    return &a != &b;
}

static Value unary(Value a, char *operator, ObjString *methodName) {
    OptValue result = callNovaMethod(a, methodName);
    if (result.hasValue)
        return result.value;
    operatorNotImplementedUnary(operator, a);
}

static bool binary(Value a, Value b, char *operator, ObjString *methodName) {
    OptValue result = callNovaMethod1arg(a, methodName, b);
    if (result.hasValue) {
        if (IS_BOOL(result.value))
            return AS_BOOL(result.value);
        reportRuntimeError("Return type must be <type bool>");
    }
    operatorNotImplemented(operator, a, b);
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
    
    operatorNotImplemented(operator, a, b);
}

bool instanceGreater(Value a, Value b) {
    return binary(a, b, ">", vm.magicStrings.gt);
}

bool instanceGreaterEqual(Value a, Value b) {
    return binary(a, b, ">=", vm.magicStrings.ge);
}

bool instanceLess(Value a, Value b) {
    return binary(a, b, "<", vm.magicStrings.lt);
}

bool instanceLessEqual(Value a, Value b) {
    return binary(a, b, "<=", vm.magicStrings.le);
}

Value instanceNot(Value a) {
    operatorNotImplementedUnary("not", a);
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

OptValue instanceGetField(Value obj, ObjString *name) {
    Value value;
    ObjInstance *instance = AS_INSTANCE(obj);

    if (tableGet(&instance->fields, name, &value))
        return (OptValue){.hasValue=true, .value=value};

    if (tableGet(&instance->class->methods, name, &value)) {
        ObjMethod *method = createMethod(obj, AS_CLOSURE(value));
        return (OptValue){.hasValue=true, .value=OBJ_VAL(method)};
    }

    return (OptValue){.hasValue=false};
}

void instanceSetField(Value obj, ObjString *name, Value value) {
    ObjInstance *instance = AS_INSTANCE(obj);
    tableSet(&instance->fields, name, value);
}

Value instanceGetAt(Value obj, Value key) {
    OptValue result = callNovaMethod1arg(obj, vm.magicStrings.getat, key);
    if (result.hasValue)
        return result.value;
    reportRuntimeError("%s is not subscriptable", decodeValueType(obj));
}

void instanceSetAt(Value obj, Value key, Value value) {
    OptValue result = callNovaMethod2args(obj, vm.magicStrings.setat, key, value);
    if (!result.hasValue)
        reportRuntimeError("%s does not support item assignment", decodeValueType(obj));
}

int instanceLen(Value value) {
    OptValue result = callNovaMethod(value, vm.magicStrings.len);
    if (result.hasValue) {
        if (isInt(result.value))
            return asInt(result.value);
        reportRuntimeError("Return type must be <type int>");
    }
    functionNotImplemented("len", value);
}

bool instanceToBool(Value value) {
    OptValue result = callNovaMethod(value, vm.magicStrings.bool_);
    if (result.hasValue) {
        if (IS_BOOL(result.value))
            return AS_BOOL(result.value);
        reportRuntimeError("Return type must be <type bool>");
    }
    functionNotImplemented("bool", value);
}

int instanceToInt(Value value) {
    OptValue result = callNovaMethod(value, vm.magicStrings.int_);
    if (result.hasValue) {
        if (isInt(result.value))
            return asInt(result.value);
        reportRuntimeError("Return type must be <type int>");
    }
    functionNotImplemented("int", value);
}

double instanceToFloat(Value value) {
    OptValue result = callNovaMethod(value, vm.magicStrings.float_);
    if (result.hasValue) {
        if (IS_NUMBER(result.value))
            return AS_NUMBER(result.value);
        reportRuntimeError("Return type must be <type number>");
    }
    functionNotImplemented("float", value);
}