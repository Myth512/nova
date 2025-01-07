#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <time.h>

#include "native.h"
#include "value.h"
#include "value_int.h"
#include "value_methods.h"
#include "object.h"
#include "object_string.h"
#include "vm.h"

Value Py_Time(int argc, Value *argv) {
    return INT_VAL((double)clock() / CLOCKS_PER_SEC);
}

Value printNative(int argc, Value *argv) {
    for (int i = 0; i < argc; i++) {
        valuePrint(argv[i]);
        putchar(i < argc - 1 ? ' ' : '\n');
    }
    return NONE_VAL;
}

#define PARSE_ARGS(...) \
    parseArgs(argc, kwargc, sizeof(keywords) / sizeof(char*), keywords, ##__VA_ARGS__)

static int stringIndex(char *keywords[], char *keyword, size_t size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(keywords[i], keyword) == 0)
            return i;
    }
    return -1;
}

static void parseArgs(int argc, int kwargc, int arity, char *keywords[], ...) {
    Value args[arity];
    for (int i = 0; i < arity; i++)
        args[i] = UNDEFINED_VAL;

    vm.top -= argc + 2 * kwargc;

    for (int i = 0; i < argc; i++) {
        if (*keywords[i] == '*') {
            size_t size = argc - i;
            ObjTuple *tuple = allocateTuple(size);
            for (int j = 0; j < size; j++) {
                tuple->values[j] = *vm.top;
                vm.top++;
            }
            args[i] = OBJ_VAL(tuple);
            i = argc;
        } else {
            args[i] = *vm.top;
            vm.top++;
        }
    }

    for (int i = 0; i < kwargc; i++) {
        Value name = vm.top[0];
        Value value = vm.top[1];
        vm.top += 2;
        int index = stringIndex(keywords, AS_STRING(name)->chars, arity);
        if (index == -1)
            reportRuntimeError("got an unexpected keyword argument '%s'", AS_STRING(name)->chars);
        if (!IS_UNDEFINED(args[index]))
            reportRuntimeError("got multiple values for argument '%s'", AS_STRING(name)->chars);
        
        args[index] = value;
    }

    va_list args1;
    va_start(args1, arity);

    for (int i = 0; i < arity; i++) {
        Value *value = va_arg(args1, Value*);
        *value = args[i];
    }

    va_end(args1);
}

Value Py_Print(int argc, int kwargc) {
    static char *keywords[] = {"*objects", "sep", "end"};
    Value objects, sep, end;
    PARSE_ARGS(&objects, &sep, &end);

    char *s;
    if (IS_UNDEFINED(sep))
        s = " ";
    else
        s = AS_STRING(sep)->chars;
    
    char *e;
    if (IS_UNDEFINED(end))
        e = "\n";
    else
        e = AS_STRING(end)->chars;

    int size = AS_TUPLE(objects)->size;

    for (int i = 0; i < size; i++) {
        valuePrint(AS_TUPLE(objects)->values[i]);
        if (i + 1 != size)
            printf("%s", s);
    }

    printf("%s", e);
    return NONE_VAL;
}

Value sqrtNative(int argc, Value *argv) {
    // if (argc != 1) {
    //     reportRuntimeError("Expect 1 argument but got %d\n", argc);
    // }
    // if (!IS_NUMBER(argv[0])) {
    //     reportRuntimeError("argument must be a number\n");
    // }
    // return NUMBER_VAL(sqrt(AS_NUMBER(argv[0])));
}

Value minNative(int argc, Value *argv) {
    // double result = AS_NUMBER(argv[0]);
    // for (int i = 1; i < argc; i++) {
    //     double cur = AS_NUMBER(argv[i]);
    //     if (cur < result)
    //         result = cur;
    // } 
    // return NUMBER_VAL(result);
}

Value maxNative(int argc, Value *argv) {
    // double result = AS_NUMBER(argv[0]);
    // for (int i = 1; i < argc; i++) {
    //     double cur = AS_NUMBER(argv[i]);
    //     if (cur > result)
    //         result = cur;
    // } 
    // return NUMBER_VAL(result);
}

Value typeNative(int argc, Value *argv) {
    // const char *type = decodeValueTypeClean(argv[0]);
    // size_t len = strlen(type);
    // ObjString *string = copyString(type, len);
    // return OBJ_VAL(string);
}

Value Py_Id(int argc, int kwargc) {
    static char *keywords[] = {"obj"};
    Value obj;
    PARSE_ARGS(&obj);

    return INT_VAL(valueId(obj));
}

Value Py_Len(int argc, int kwargc) {
    static char *keywords[] = {"obj"};
    Value obj;
    PARSE_ARGS(&obj);

    return INT_VAL(valueLen(obj));
}

Value Py_Input(int argc, int kwargc) {
    static char *keywords[] = {"promt"};
    Value promt;
    PARSE_ARGS(&promt);

    if (!IS_UNDEFINED(promt))
        valuePrint(promt);

    const size_t size = 256;
    char buffer[size];

    if (fgets(buffer, size, stdin) == NULL)
        reportRuntimeError("Can't read input");
    
    size_t length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n')
        buffer[--length] = '\0';

    ObjString *string = copyString(buffer, length);

    return STRING_VAL(string);
}

Value Py_Hex(int argc, int kwargc) {
    static char *keywords[] = {"number"};
    Value number;
    PARSE_ARGS(&number);

    if (!IS_INT(number))
        reportRuntimeError("expect int");

    char buffer[19];
    int len = snprintf(buffer, sizeof(buffer), "0x%llx", AS_INT(number));

    ObjString *res = copyString(buffer, len + 1);
    return OBJ_VAL(res);
}


Value Py_IsInstance(int argc, int kwargc) {
    static char *keywords[] = {"obj", "class"};
    Value obj, class;
    PARSE_ARGS(&obj, &class);

    return BOOL_VAL(isInstance(obj, class));
}