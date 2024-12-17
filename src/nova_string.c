#include <string.h>

#include "nova_string.h"
#include "object.h"
#include "object_string.h"
#include "vm.h"
#include "error.h"

Value novaStringReverse(int argc, Value *argv) {

}

Value novaStringReplace(int argc, Value *argv) {

}

Value novaStringEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);

    if (!IS_STRING(argv[1]))
        return BOOL_VAL(false);

    ObjString *a = AS_STRING(argv[0]);
    ObjString *b = AS_STRING(argv[1]);

    ObjString *c;

    if (a->isInterned && b->isInterned)
        return BOOL_VAL(a == b);

    if (a->length != b->length) 
        return BOOL_VAL(false);

    return BOOL_VAL(memcmp(a->chars, b->chars, a->length) == 0);
}

Value novaStringNotEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);

    if (!IS_STRING(argv[1]))
        return BOOL_VAL(true);

    ObjString *a = AS_STRING(argv[0]);
    ObjString *b = AS_STRING(argv[1]);

    if (a->isInterned && b->isInterned)
        return BOOL_VAL(a != b);

    if (a->length != b->length) 
        return BOOL_VAL(true);

    return BOOL_VAL(memcmp(a->chars, b->chars, a->length) != 0);
}

Value novaStringGreater(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);

    if (!IS_STRING(argv[1]))
        reportRuntimeError("Unsupported operator '>' for %s and %s", decodeValueType(argv[0]), decodeValueType(argv[1]));

    ObjString *a = AS_STRING(argv[0]);
    ObjString *b = AS_STRING(argv[1]);

    return BOOL_VAL(memcmp(a->chars, b->chars, a->length) > 0);
}

Value novaStringGreaterEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);

    if (!IS_STRING(argv[1]))
        reportRuntimeError("Unsupported operator '>=' for %s and %s", decodeValueType(argv[0]), decodeValueType(argv[1]));

    ObjString *a = AS_STRING(argv[0]);
    ObjString *b = AS_STRING(argv[1]);

    return BOOL_VAL(memcmp(a->chars, b->chars, a->length) >= 0);
}

Value novaStringLess(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);

    if (!IS_STRING(argv[1]))
        reportRuntimeError("Unsupported operator '<' for %s and %s", decodeValueType(argv[0]), decodeValueType(argv[1]));

    ObjString *a = AS_STRING(argv[0]);
    ObjString *b = AS_STRING(argv[1]);

    return BOOL_VAL(memcmp(a->chars, b->chars, a->length) < 0);
}

Value novaStringLessEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);

    if (!IS_STRING(argv[1]))
        reportRuntimeError("Unsupported operator '<=' for %s and %s", decodeValueType(argv[0]), decodeValueType(argv[1]));

    ObjString *a = AS_STRING(argv[0]);
    ObjString *b = AS_STRING(argv[1]);

    return BOOL_VAL(memcmp(a->chars, b->chars, a->length) <= 0);
}

Value novaStringAdd(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);

    if (!IS_STRING(argv[1]))
        reportRuntimeError("Unsupported operator '+' for %s and %s", decodeValueType(argv[0]), decodeValueType(argv[1]));

    ObjString *a = AS_STRING(argv[0]);
    ObjString *b = AS_STRING(argv[1]);


    return OBJ_VAL(result);
}

Value novaStringMultiply(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);

    if (!isInt(argv[1]))
        reportRuntimeError("Multiplier must be whole number");

    ObjString *string = AS_STRING(argv[0]);
    int multiplier = asInt(argv[1]);


    return OBJ_VAL(result);
}

Value novaStringGetAt(int argc, Value *argv) {

}

Value novaStringLen(int argc, Value *argv) {
    if (argc != 0)
        reportArityError(0, argc);

    return NUMBER_VAL(AS_STRING(argv[0])->length);
}

Value novaStringHash(int argc, Value *argv) {

}