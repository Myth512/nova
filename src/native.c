#include <stdio.h>
#include <math.h>
#include <time.h>

#include "native.h"

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