#include <string.h>

#include "py_string.h"
#include "value_int.h"
#include "object.h"
#include "object_string.h"
#include "vm.h"
#include "error.h"

static Value binary(int argc, Value *argv, char *name, Value (*func)(Value, Value)) {
    if (argc != 1)
        reportArityError(1, 1, argc);

    Value res = func(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented(name, argv[0], argv[1]);
    
    return res;
}

Value pyStringReverse(int argc, Value *argv) {

}

Value pyStringReplace(int argc, Value *argv) {

}

Value pyStringEqual(int argc, Value *argv) {
    return binary(argc, argv, "==", stringEqual);
}

Value pyStringNotEqual(int argc, Value *argv) {
    return binary(argc, argv, "!=", stringNotEqual);
}

Value pyStringGreater(int argc, Value *argv) {
    return binary(argc, argv, ">", stringGreater);
}

Value pyStringGreaterEqual(int argc, Value *argv) {
    return binary(argc, argv, ">=", stringGreaterEqual);
}

Value pyStringLess(int argc, Value *argv) {
    return binary(argc, argv, "<", stringLess);
}

Value pyStringLessEqual(int argc, Value *argv) {
    return binary(argc, argv, "<=", stringLessEqual);
}

Value pyStringAdd(int argc, Value *argv) {
    return binary(argc, argv, "+", stringAdd);
}

Value pyStringMultiply(int argc, Value *argv) {
    return binary(argc, argv, "*", stringMultiply);
}

Value pyStringGetAt(int argc, Value *argv) {

}