#include <stdio.h>

#include "object_utils.h"
#include "object_string.h"
#include "object_array.h"
#include "object_class.h"
#include "vm.h"

static void printFunction(ObjFunction *function) {
    if (function->name == NULL) {
        printf("<script>");
        return;
    }
    printf("<function %s>", function->name->chars);
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_CLOSURE:
            printFunction(AS_CLOSURE(value)->function);
            break;
        case OBJ_FUNCTION:
            printFunction(AS_FUNCTION(value));
            break;
        case OBJ_NATIVE:
            printf("<native function '%s'>", AS_NATIVE(value)->name);
            break;
        case OBJ_NATIVE_METHOD:
            printf("<native method '%s'>", AS_NATIVE_METHOD(value)->name);
            break;
        case OBJ_STRING:
            printf("%s", AS_CSTRING(value));
            break;
        case OBJ_RAW_STRING: {
            ObjRawString *string = AS_RAW_STRING(value);
            printf("%.*s\n", string->length, string->chars);
            break;
        }
        case OBJ_UPVALUE:
            printValue(*(((ObjUpvalue*)AS_OBJ(value))->location));
            break;
        case OBJ_ARRAY:
            arrayPrint(AS_ARRAY(value));
            break;
        case OBJ_CLASS:
            printf("%s", AS_CLASS(value)->name->chars);
            break;
        case OBJ_INSTANCE:
            instancePrint(value);
            break;
        case OBJ_METHOD:
            printFunction(AS_METHOD(value)->method->function);
            break;
    }
}

int writeObject(Value value, char *buffer, const size_t maxSize) {
    switch (OBJ_TYPE(value)) {
        case OBJ_FUNCTION:
            ObjFunction *function = AS_FUNCTION(value);
            return snprintf(buffer, maxSize, "<function %s>", function->name->chars);
        case OBJ_NATIVE:
            return snprintf(buffer, maxSize, "<native function %s>", AS_NATIVE(value)->name);
        case OBJ_STRING:
            return snprintf(buffer, maxSize, "%s", AS_CSTRING(value));
        case OBJ_RAW_STRING:
            return writeRawstring(buffer, AS_RAW_STRING(value));
    }
    return -1; // unreachable
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

static Value equality(Value a, Value b, bool typeMismatchValue, Value (*instanceFunc)(Value, Value), bool (*stringFunc)(ObjString*, ObjString*), bool (*arrayFunc)(ObjArray*, ObjArray*)) {
    if (IS_INSTANCE(a) || IS_INSTANCE(b))
        return instanceFunc(a, b);
    
    if (OBJ_TYPE(a) != OBJ_TYPE(b))
        return BOOL_VAL(typeMismatchValue);

    switch (OBJ_TYPE(a)) {
        case OBJ_STRING:
            return BOOL_VAL(stringFunc(AS_STRING(a), AS_STRING(b)));
        case OBJ_ARRAY:
            return BOOL_VAL(arrayFunc(AS_ARRAY(a), AS_ARRAY(b)));
        default:
            reportTypeError("==", a, b);
    }
}

Value objectEqual(Value a, Value b) {
    return equality(a, b, false, instanceEqual, stringEqual, arrayEqual);
}

Value objectNotEqual(Value a, Value b) {
    return equality(a, b, true, instanceNotEqual, stringNotEqual, arrayNotEqual);
}

static Value unary(Value a, char *name, Value (*instanceFunc)(Value)) {
    switch (OBJ_TYPE(a)) {
        case OBJ_INSTANCE:
            return instanceFunc(a);
        default:
            reportTypeError1op(name, a);
    }
}

static Value binary(Value a, Value b, char *name, Value (*instanceFunc)(Value, Value), bool (*stringFunc)(ObjString*, ObjString*), bool (*arrayFunc)(ObjArray*, ObjArray*)) {
    if (IS_INSTANCE(a) || IS_INSTANCE(b))
        return instanceFunc(a, b);
    
    if (OBJ_TYPE(a) != OBJ_TYPE(b))
        reportTypeError(name, a, b);

    switch (OBJ_TYPE(a)) {
        case OBJ_STRING:
            if (!stringFunc)
                reportTypeError(name, a, b);
            return BOOL_VAL(stringFunc(AS_STRING(a), AS_STRING(b)));
        case OBJ_ARRAY:
            if (!arrayFunc)
                reportTypeError(name, a, b);
            return BOOL_VAL(arrayFunc(AS_ARRAY(a), AS_ARRAY(b)));
        default:
            reportTypeError(name, a, b);
    }
}

Value objectGreater(Value a, Value b) {
    return binary(a, b, ">", instanceGreater, stringGreater, arrayGreater);
}

Value objectGreaterEqual(Value a, Value b) {
    return binary(a, b, ">=", instanceGreaterEqual, stringGreaterEqual, arrayGreaterEqual);
}

Value objectLess(Value a, Value b) {
    return binary(a, b, "<", instanceLess, stringLess, arrayLess);
}

Value objectLessEqual(Value a, Value b) {
    return binary(a, b, "<=", instanceLessEqual, stringLessEqual, arrayLessEqual);
}

Value objectNot(Value a) {
    return unary(a, "not", instanceNot);
}

Value objectAdd(Value a, Value b) {
    if (IS_STRING(a) && IS_STRING(b))
        return OBJ_VAL(stringAdd(AS_STRING(a), AS_STRING(b)));
    return binary(a, b, "+", instanceAdd, NULL, NULL);
}

Value objectSubtract(Value a, Value b) {
    return binary(a, b, "-", instanceSubtract, NULL, NULL);
}

Value objectMultiply(Value a, Value b) {
    if (IS_STRING(a) && isInt(b))
        return OBJ_VAL(stringMultiply(AS_STRING(a), asInt(b)));
    if (isInt(a) && IS_STRING(b))
        return OBJ_VAL(stringMultiply(AS_STRING(b), asInt(a)));
    return binary(a, b, "*", instanceMultiply, NULL, NULL);
}

Value objectDivide(Value a, Value b) {
    return binary(a, b, "/", instanceDivide, NULL, NULL);
}

Value objectModulo(Value a, Value b) {
    return binary(a, b, "%%", instanceModulo, NULL, NULL);
}

Value objectPower(Value a, Value b) {
    return binary(a, b, "^", instancePower, NULL, NULL);
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

uint64_t objectHash(Value a) {

}