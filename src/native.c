#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "native.h"
#include "value.h"
#include "object.h"
#include "error.h"

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
    const char *type = decodeValueType(argv[0]);
    size_t len = strlen(type);
    ObjString *string = allocateString(type, len);
    return OBJ_VAL(string);
}

Value lenNative(int argc, Value *argv) {
    if (argc != 1) {
        reportRuntimeError("Expect 1 argument but got %d\n", argc);
    }
    Value value = argv[0];
    if (!IS_OBJ(value)) {
        reportRuntimeError("%s does not have len", decodeValueType(value));
    }
    switch (AS_OBJ(value)->type) {
        case OBJ_ARRAY:
            return NUMBER_VAL(AS_ARRAY(value)->values.size);
        case OBJ_STRING:
            return NUMBER_VAL(AS_STRING(value)->length);
        default:
            reportRuntimeError("%s does not have len", decodeValueType(value));
    }
}

Value hashNative(int argc, Value *argv) {
    if (argc != 1) {
        reportRuntimeError("Expect 1 argument but got %d\n", argc);
    }
    return NUMBER_VAL(hashValue(argv[0]));
}

Value arrayPushNative(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[-1]);
    pushValue(&array->values, argv[0]);
    return NIL_VAL;
}

Value arrayPopNative(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[-1]);
    if (array->values.size == 0) {
        reportRuntimeError("Can't pop from empty array");
    }
    return popValue(&array->values);
}

Value arrayInsertNative(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[-1]);
    int index = AS_NUMBER(argv[0]);
    insertValue(&array->values, index, argv[1]);
    return NIL_VAL;
}

Value arrayReverseNative(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[-1]);
    reverseValueVec(&array->values);
    return NIL_VAL;
}

Value arraySortNative(int argc, Value *argv) {

}