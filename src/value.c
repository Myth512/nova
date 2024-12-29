#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "value.h"
#include "object.h"
#include "object_string.h"
#include "object_class.h"
#include "memory.h"
#include "error.h"
#include "vm.h"

int writeToBuffer(char *buffer, const size_t size, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int bytesWritten;

    if (buffer == NULL)
        bytesWritten = vprintf(format, args);
    else
        bytesWritten = vsnprintf(buffer, size, format, args);
    
    va_end(args);

    return bytesWritten;
}

const char* decodeValueType(Value value) {
    // switch (value.type) {
    //     case VAL_BOOL:
    //         return "<type bool>";
    //     case VAL_NIL:
    //         return "<type nil>";
    //     case VAL_NUMBER:
    //         return "<type number>";
    //     case VAL_OBJ:
    //         return decodeObjType(value);
    // }
    // return ""; // uncreachable
}

const char *decodeValueTypeClean(Value value) {
    // switch (value.type) {
    //     case VAL_BOOL:
    //         return "bool";
    //     case VAL_NIL:
    //         return "nil";
    //     case VAL_NUMBER:
    //         return "number";
    //     case VAL_OBJ:
    //         return decodeObjTypeClean(value);
    // }
    // return ""; // uncreachable
}