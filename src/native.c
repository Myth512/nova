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

Value Py_Repr(int argc, int kwargc) {
    static char *keywords[] = {"obj"};
    Value obj;
    PARSE_ARGS(&obj);

    return OBJ_VAL(valueToRepr(obj));
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

Value Py_Iter(int argc, int kwargc) {
    static char *keywords[] = {"object"};
    Value object;
    PARSE_ARGS(&object);

    return valueIter(object);
}

Value Py_Next(int argc, int kwargc) {
    static char *keywords[] = {"object"};
    Value object;
    PARSE_ARGS(&object);

    return valueNext(object);
}