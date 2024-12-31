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

Value PyString_Reverse(int argc, Value *argv) {

}

Value PyString_Replace(int argc, Value *argv) {

}

Value PyString_Equal(int argc, Value *argv) {
    return binary(argc, argv, "==", String_Equal);
}

Value PyString_NotEqual(int argc, Value *argv) {
    return binary(argc, argv, "!=", String_NotEqual);
}

Value PyString_Greater(int argc, Value *argv) {
    return binary(argc, argv, ">", String_Greater);
}

Value PyString_GreaterEqual(int argc, Value *argv) {
    return binary(argc, argv, ">=", String_GreaterEqual);
}

Value PyString_Less(int argc, Value *argv) {
    return binary(argc, argv, "<", String_Less);
}

Value PyString_LessEqual(int argc, Value *argv) {
    return binary(argc, argv, "<=", String_LessEqual);
}

Value PyString_Add(int argc, Value *argv) {
    return binary(argc, argv, "+", String_Add);
}

Value PyString_Multiply(int argc, Value *argv) {
    return binary(argc, argv, "*", String_Multiply);
}

Value PyString_GetAt(int argc, Value *argv) {

}

Value PyString_Hash(int argc, Value *argv) {

}

Value PyString_Len(int argc, Value *argv) {

}
