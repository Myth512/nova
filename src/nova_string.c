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

    return BOOL_VAL(stringEqual(a, b));
}

Value novaStringNotEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);

    if (!IS_STRING(argv[1]))
        return BOOL_VAL(true);

    ObjString *a = AS_STRING(argv[0]);
    ObjString *b = AS_STRING(argv[1]);

    return BOOL_VAL(stringNotEqual(a, b));
}

static Value inequality(int argc, Value *argv, char *name, bool (*func)(ObjString*, ObjString*)) {
    if (argc != 1)
        reportArityError(1, argc);

    if (!IS_STRING(argv[1]))
        operatorNotImplemented(name, argv[0], argv[1]);

    ObjString *a = AS_STRING(argv[0]);
    ObjString *b = AS_STRING(argv[1]);
    
    return BOOL_VAL(func(a, b));
}

Value novaStringGreater(int argc, Value *argv) {
    return inequality(argc, argv, ">", stringGreater);
}

Value novaStringGreaterEqual(int argc, Value *argv) {
    return inequality(argc, argv, ">=", stringGreaterEqual);
}

Value novaStringLess(int argc, Value *argv) {
    return inequality(argc, argv, "<", stringLess);
}

Value novaStringLessEqual(int argc, Value *argv) {
    return inequality(argc, argv, "<=", stringLessEqual);
}

Value novaStringAdd(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);

    if (!IS_STRING(argv[1]))
        operatorNotImplemented("+", argv[0], argv[1]);

    ObjString *a = AS_STRING(argv[0]);
    ObjString *b = AS_STRING(argv[1]);

    return OBJ_VAL(stringAdd(a, b));
}

Value novaStringMultiply(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);

    if (!isInt(argv[1]))
        operatorNotImplemented("*", argv[0], argv[1]);
    
    ObjString *string = AS_STRING(argv[0]);
    int scalar = asInt(argv[1]);

    return OBJ_VAL(stringMultiply(string, scalar));
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