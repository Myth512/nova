#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "native.h"
#include "value.h"
#include "object.h"
#include "object_string.h"
#include "vm.h"

Value clockNative(int argc, Value *argv) {
    return NUMBER_VAL((double)clock() / CLOCKS_PER_SEC);
}

Value printNative(int argc, Value *argv) {
    for (int i = 0; i < argc; i++) {
        valuePrint(argv[i]);
        putchar(i < argc - 1 ? ' ' : '\n');
    }
    return NIL_VAL;
}

Value sqrtNative(int argc, Value *argv) {
    if (argc != 1) {
        reportRuntimeError("Expect 1 argument but got %d\n", argc);
    }
    if (!IS_NUMBER(argv[0])) {
        reportRuntimeError("argument must be a number\n");
    }
    return NUMBER_VAL(sqrt(AS_NUMBER(argv[0])));
}

Value minNative(int argc, Value *argv) {
    double result = AS_NUMBER(argv[0]);
    for (int i = 1; i < argc; i++) {
        double cur = AS_NUMBER(argv[i]);
        if (cur < result)
            result = cur;
    } 
    return NUMBER_VAL(result);
}

Value maxNative(int argc, Value *argv) {
    double result = AS_NUMBER(argv[0]);
    for (int i = 1; i < argc; i++) {
        double cur = AS_NUMBER(argv[i]);
        if (cur > result)
            result = cur;
    } 
    return NUMBER_VAL(result);
}

Value typeNative(int argc, Value *argv) {
    const char *type = decodeValueTypeClean(argv[0]);
    size_t len = strlen(type);
    ObjString *string = copyString(type, len);
    return OBJ_VAL(string);
}

Value novaAddr(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);
    return NUMBER_VAL(valueAddr(argv[0]));
}

Value novaLen(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);
    return NUMBER_VAL(valueLen(argv[0]));
}

Value novaBool(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);
    return BOOL_VAL(valueToBool(argv[0]));
}

Value novaInt(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);
    return NUMBER_VAL(valueToInt(argv[0]));
}

Value novaFloat(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);
    return NUMBER_VAL(valueToFloat(argv[0]));
}

Value novaStr(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, argc);
    return OBJ_VAL(valueToStr(argv[0]));
}