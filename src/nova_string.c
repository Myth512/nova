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
    if (argc != 1) {
        reportArityError(1, argc);
        printErrorInCode();
    }
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
        if (argc != 1) {
        reportArityError(1, argc);
        printErrorInCode();
    }
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

Value stringGreater(int argc, Value *argv) {
    if (argc != 1) {
        reportArityError(1, argc);
        printErrorInCode();
    }
    if (!IS_STRING(argv[1])) {
        reportRuntimeError("Unsupported operator '>' for %s and %s", decodeValueType(argv[0]), decodeValueType(argv[1]));
        printErrorInCode();
    }

    ObjString *a = AS_STRING(argv[0]);
    ObjString *b = AS_STRING(argv[1]);

    return BOOL_VAL(memcmp(a->chars, b->chars, a->length) > 0);
}

Value stringGreaterEqual(int argc, Value *argv) {
    if (argc != 1) {
        reportArityError(1, argc);
        printErrorInCode();
    }
    if (!IS_STRING(argv[1])) {
        reportRuntimeError("Unsupported operator '>=' for %s and %s", decodeValueType(argv[0]), decodeValueType(argv[1]));
        printErrorInCode();
    }

    ObjString *a = AS_STRING(argv[0]);
    ObjString *b = AS_STRING(argv[1]);

    return BOOL_VAL(memcmp(a->chars, b->chars, a->length) >= 0);
}

Value stringLess(int argc, Value *argv) {
    if (argc != 1) {
        reportArityError(1, argc);
        printErrorInCode();
    }
    if (!IS_STRING(argv[1])) {
        reportRuntimeError("Unsupported operator '<' for %s and %s", decodeValueType(argv[0]), decodeValueType(argv[1]));
        printErrorInCode();
    }

    ObjString *a = AS_STRING(argv[0]);
    ObjString *b = AS_STRING(argv[1]);

    return BOOL_VAL(memcmp(a->chars, b->chars, a->length) < 0);
}

Value stringLessEqual(int argc, Value *argv) {
    if (argc != 1) {
        reportArityError(1, argc);
        printErrorInCode();
    }
    if (!IS_STRING(argv[1])) {
        reportRuntimeError("Unsupported operator '<=' for %s and %s", decodeValueType(argv[0]), decodeValueType(argv[1]));
        printErrorInCode();
    }
    ObjString *a = AS_STRING(argv[0]);
    ObjString *b = AS_STRING(argv[1]);

    return BOOL_VAL(memcmp(a->chars, b->chars, a->length) <= 0);
}

Value stringAdd(int argc, Value *argv) {
    if (argc != 1) {
        reportArityError(1, argc);
        printErrorInCode();
    }
    if (!IS_STRING(argv[1])) {
        reportRuntimeError("Unsupported operator '+' for %s and %s", decodeValueType(argv[0]), decodeValueType(argv[1]));
        printErrorInCode();
    }

    ObjString *a = AS_STRING(argv[0]);
    ObjString *b = AS_STRING(argv[1]);

    int length = a->length + b->length;

    ObjString *result = allocateString(length);

    memcpy(result->chars, a->chars, a->length);
    memcpy(result->chars + a->length, b->chars, b->length);
    result->chars[length] = '\0';

    return OBJ_VAL(result);
}

Value stringMultiply(int argc, Value *argv) {
    if (argc != 1) {
        reportArityError(1, argc);
        printErrorInCode();
    }

    if (!isInt(argv[1])) {
        reportRuntimeError("Multiplier must be whole number");
        printErrorInCode();
    }

    ObjString *string = AS_STRING(argv[0]);
    int multiplier = asInt(argv[1]);

    size_t oldLength = string->length;
    size_t newLength = oldLength * multiplier;

    ObjString *result = allocateString(newLength);

    for (int i = 0; i < multiplier; i++) {
        memcpy(result->chars + i * oldLength, string->chars, oldLength);
    }

    return OBJ_VAL(result);
}

Value stringGetAt(int argc, Value *argv) {

}

Value stringLen(int argc, Value *argv) {
    if (argc != 0) {
        reportArityError(0, argc);
        printErrorInCode();
    }
    return NUMBER_VAL(AS_STRING(argv[0])->length);
}

Value stringHash(int argc, Value *argv) {

}