#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "native.h"
#include "value.h"
#include "object.h"

Value clockNative(int argc, Value *argv) {
    return NUMBER_VAL((double)clock() / CLOCKS_PER_SEC);
}

Value printNative(int argc, Value *argv) {
    for (int i = 0; i < argc; i++) {
        printValue(argv[i]);
        putchar(i < argc - 1 ? ' ' : '\n');
    }
    return NIL_VAL;
}

Value sqrtNative(int argc, Value *argv) {
    if (argc != 1) {
        printf("expect 1 argument but got %d\n", argc);
    }
    if (!IS_NUMBER(argv[0])) {
        printf("argument must be a number\n");
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
    const char *type = decodeValueType(argv[0]);
    size_t len = strlen(type);
    ObjString *string = allocateString(type, len);
    return OBJ_VAL(string);
}

Value lenNative(int argc, Value *argv) {
    if (argc != 1) {
        printf("Expect 1 argument but got %d\n", argc);
    }
    Value value = argv[0];
    if (!IS_OBJ(value)) {
        printf("%s does not have len", decodeValueType(value));
    }
    switch (AS_OBJ(value)->type) {
        case OBJ_ARRAY:
            return NUMBER_VAL(AS_ARRAY(value)->values.size);
        case OBJ_STRING:
            return NUMBER_VAL(AS_STRING(value)->length);
        default:
            printf("%s does not have len", decodeValueType(value));
    }
}