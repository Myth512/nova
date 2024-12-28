#include <stdio.h>

#include "object_utils.h"
#include "object_string.h"
#include "object_array.h"
#include "object_tuple.h"
#include "object_class.h"
#include "object_function.h"
#include "vm.h"

int objectWrite(Value value, char *buffer, const size_t size) {
    switch (OBJ_TYPE(value)) {
        case OBJ_CLOSURE:
            return writeToBuffer(buffer, size, "<function %s>", AS_CLOSURE(value)->function->name->chars);
        case OBJ_FUNCTION:
            return writeToBuffer(buffer, size, "<function %s>", AS_FUNCTION(value)->name->chars);
        case OBJ_NATIVE:
            return writeToBuffer(buffer, size, "<native function '%s'>", AS_NATIVE(value)->name);
        case OBJ_NATIVE_METHOD:
            return writeToBuffer(buffer, size, "<native method '%s'>", AS_NATIVE_METHOD(value)->name);
        case OBJ_STRING:
            return writeToBuffer(buffer, size, "%s", AS_CSTRING(value));
        case OBJ_RAW_STRING: {
            ObjRawString *string = AS_RAW_STRING(value);
            return writeToBuffer(buffer, size, "%.*s", string->length, string->chars);
        }
        case OBJ_UPVALUE: {
            ObjUpvalue *upvalue = AS_UPVALUE(value);
            return valueWrite(*upvalue->location, buffer, size);
        }
        case OBJ_ARRAY:
            return arrayWrite(AS_ARRAY(value), buffer, size);
        case OBJ_TUPLE:
            return tupleWrite(AS_TUPLE(value), buffer, size);
        case OBJ_CLASS:
            return writeToBuffer(buffer, size, "%s", AS_CLASS(value)->name->chars);
        case OBJ_INSTANCE:
            return instanceWrite(value, buffer, size);
        case OBJ_METHOD:
            return writeToBuffer(buffer, size, "<method %s>", AS_METHOD(value)->method->function->name->chars);
    }
}

int objectPrint(Value value) {
    return objectWrite(value, NULL, 0);
}

const char* decodeObjType(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_NATIVE:
            return "<type native>";
        case OBJ_FUNCTION:
            return "<type function>";
        case OBJ_STRING:
            return "<type string>";
        case OBJ_RAW_STRING:
            return "<type raw string>";
        case OBJ_UPVALUE:
            return "<type upvalue>";
        case OBJ_CLOSURE:
            return "<type closure>";
        case OBJ_ARRAY:
            return "<type array>";
        case OBJ_CLASS:
            return "<type class>";
        case OBJ_INSTANCE:
            return "<type instance>";
    }
    return "invalid type"; // uncreachable
}

const char *decodeObjTypeClean(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_NATIVE:
            return "native";
        case OBJ_STRING:
            return "string";
        case OBJ_UPVALUE:
            return "upvalue";
        case OBJ_CLOSURE:
            return "function";
        case OBJ_ARRAY:
            return "array";
        case OBJ_CLASS:
            return "class";
        case OBJ_INSTANCE:
            return AS_INSTANCE(value)->class->name->chars;
    }
    return "invalid type"; // uncreachable
}

uint64_t hashObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_NATIVE:
            return hashLong((long)AS_NATIVE(value)->function);
        case OBJ_CLOSURE:
            return hashLong((long)AS_CLOSURE(value)->function);
        default:
            return 0;
    }
}

static bool equality(Value a, Value b, bool typeMismatchValue, bool (*instanceFunc)(Value, Value), bool (*stringFunc)(ObjString*, ObjString*), bool (*arrayFunc)(ObjArray*, ObjArray*)) {
    if (IS_INSTANCE(a) || IS_INSTANCE(b))
        return instanceFunc(a, b);
    
    if (OBJ_TYPE(a) != OBJ_TYPE(b))
        return typeMismatchValue;

    switch (OBJ_TYPE(a)) {
        case OBJ_STRING:
            return stringFunc(AS_STRING(a), AS_STRING(b));
        case OBJ_ARRAY:
            return arrayFunc(AS_ARRAY(a), AS_ARRAY(b));
        default:
            operatorNotImplemented("==", a, b);
    }
}

bool objectEqual(Value a, Value b) {
    return equality(a, b, false, instanceEqual, stringEqual, arrayEqual);
}

bool objectNotEqual(Value a, Value b) {
    return equality(a, b, true, instanceNotEqual, stringNotEqual, arrayNotEqual);
}

static Value unary(Value a, char *name, Value (*instanceFunc)(Value)) {
    switch (OBJ_TYPE(a)) {
        case OBJ_INSTANCE:
            return instanceFunc(a);
        default:
            operatorNotImplementedUnary(name, a);
    }
}

static bool inequality(Value a, Value b, char *name, bool (*instanceFunc)(Value, Value), bool (*stringFunc)(ObjString*, ObjString*), bool (*arrayFunc)(ObjArray*, ObjArray*)) {
    if (IS_INSTANCE(a) || IS_INSTANCE(b))
        return instanceFunc(a, b);
    
    if (OBJ_TYPE(a) != OBJ_TYPE(b))
        operatorNotImplemented(name, a, b);

    switch (OBJ_TYPE(a)) {
        case OBJ_STRING:
            return stringFunc(AS_STRING(a), AS_STRING(b));
        case OBJ_ARRAY:
            return arrayFunc(AS_ARRAY(a), AS_ARRAY(b));
        default:
            operatorNotImplemented(name, a, b);
    }
}

static Value arithmetic(Value a, Value b, char *operator, Value (*instanceFunc)(Value, Value)) {
    if (IS_INSTANCE(a) || IS_INSTANCE(b))
        return instanceFunc(a, b);
    
    if (OBJ_TYPE(a) != OBJ_TYPE(b))
        operatorNotImplemented(operator, a, b);

    switch (OBJ_TYPE(a)) {
        default:
            operatorNotImplemented(operator, a, b);
    }
}

bool objectGreater(Value a, Value b) {
    return inequality(a, b, ">", instanceGreater, stringGreater, arrayGreater);
}

bool objectGreaterEqual(Value a, Value b) {
    return inequality(a, b, ">=", instanceGreaterEqual, stringGreaterEqual, arrayGreaterEqual);
}

bool objectLess(Value a, Value b) {
    return inequality(a, b, "<", instanceLess, stringLess, arrayLess);
}

bool objectLessEqual(Value a, Value b) {
    return inequality(a, b, "<=", instanceLessEqual, stringLessEqual, arrayLessEqual);
}

Value objectAdd(Value a, Value b) {
    if (IS_STRING(a) && IS_STRING(b))
        return OBJ_VAL(stringAdd(AS_STRING(a), AS_STRING(b)));
    if (IS_ARRAY(a) && IS_ARRAY(b))
        return OBJ_VAL(arrayAdd(AS_ARRAY(a), AS_ARRAY(b)));
    return arithmetic(a, b, "+", instanceAdd);
}

Value objectSubtract(Value a, Value b) {
    return arithmetic(a, b, "-", instanceSubtract);
}

Value objectMultiply(Value a, Value b) {
    if (IS_STRING(a) && isInt(b))
        return OBJ_VAL(stringMultiply(AS_STRING(a), asInt(b)));
    if (isInt(a) && IS_STRING(b))
        return OBJ_VAL(stringMultiply(AS_STRING(b), asInt(a)));
    if (IS_ARRAY(a) && isInt(b))
        return OBJ_VAL(arrayMultiply(AS_ARRAY(a), asInt(b)));
    if (isInt(a) && IS_ARRAY(b))
        return OBJ_VAL(arrayMultiply(AS_ARRAY(b), asInt(a)));
    return arithmetic(a, b, "*", instanceMultiply);
}

Value objectDivide(Value a, Value b) {
    return arithmetic(a, b, "/", instanceDivide);
}

Value objectModulo(Value a, Value b) {
    return arithmetic(a, b, "%%", instanceModulo);
}

Value objectPower(Value a, Value b) {
    return arithmetic(a, b, "^", instancePower);
}

Value objectNegate(Value a) {
    return unary(a, "-", instanceNegate);
}

Value objectIncrement(Value a) {
    return unary(a, "++", instanceIncrement);
}

Value objectDecrement(Value a) {
    return unary(a, "--", instanceDecrement);
}

OptValue objectGetField(Value obj, ObjString *name) {
    switch (OBJ_TYPE(obj)) {
        case OBJ_STRING:
            return stringGetField(obj, name);
        case OBJ_ARRAY:
            return arrayGetField(obj, name);
        case OBJ_INSTANCE:
            return instanceGetField(obj, name);
        default:
            reportRuntimeError("%s does not have any fields", decodeValueType(obj));
    }
}

void objectSetField(Value obj, ObjString *name, Value value) {
    switch (OBJ_TYPE(obj)) {
        case OBJ_INSTANCE:
            instanceSetField(obj, name, value);
            break;
        case OBJ_STRING:
        case OBJ_ARRAY:
            reportRuntimeError("%s fields are immutable", decodeValueType(obj));
        default:
            reportRuntimeError("%s does not have any fields", decodeValueType(obj));
    }
}

Value objectGetAt(Value obj, Value key) {
    switch (OBJ_TYPE(obj)) {
        case OBJ_STRING:
            return stringGetAt(AS_STRING(obj), key);
        case OBJ_ARRAY:
            return arrayGetAt(AS_ARRAY(obj), key);
        case OBJ_INSTANCE:
            return instanceGetAt(obj, key);
        default:
            reportRuntimeError("%s is not subscriptable", decodeValueType(obj));
    }
}

void objectSetAt(Value obj, Value key, Value value) {
    switch (OBJ_TYPE(obj)) {
        case OBJ_ARRAY:
            arraySetAt(AS_ARRAY(obj), key, value);
            break;
        case OBJ_INSTANCE:
            instanceSetAt(obj, key, value);
            break;
        case OBJ_STRING:
            reportRuntimeError("%s is immutable", decodeValueType(obj));
        default:
            reportRuntimeError("%s does not support item assignment", decodeValueType(obj));
    }
}

uint64_t objectHash(Value a) {

}

int objectLen(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            return stringLen(AS_STRING(value));
        case OBJ_ARRAY:
            return arrayLen(AS_ARRAY(value));
        case OBJ_INSTANCE:
            return instanceLen(value);
        default:
            functionNotImplemented("len", value);
    }
}

bool objectToBool(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            return stringToBool(AS_STRING(value));
        case OBJ_ARRAY:
            return arrayToBool(AS_ARRAY(value));
        case OBJ_INSTANCE:
            return instanceToBool(value); 
        default:
            functionNotImplemented("bool", value);
    }
}

int objectToInt(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            return stringToInt(AS_STRING(value));
        case OBJ_INSTANCE:
            return instanceToInt(value);
        default:
            functionNotImplemented("int", value);
    }
}

double objectToFloat(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            return stringToFloat(AS_STRING(value));
        case OBJ_INSTANCE:
            return instanceToInt(value);
        default:
            functionNotImplemented("float", value);
    }
}
