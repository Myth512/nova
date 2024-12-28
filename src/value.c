#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "value.h"
#include "object.h"
#include "object_utils.h"
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

int valueWrite(Value value, char *buffer, const size_t size) {
    switch (value.type) {
        case VAL_BOOL:
            return writeToBuffer(buffer, size, AS_BOOL(value) ? "true" : "false");
        case VAL_NIL:
            return writeToBuffer(buffer, size, "nil");
        case VAL_UNDEFINED:
            return writeToBuffer(buffer, size, "undefined");
        case VAL_INT:
            return writeToBuffer(buffer, size, "%lld", AS_INT(value));
        case VAL_FLOAT:
            return writeToBuffer(buffer, size, "%g", AS_FLOAT(value));
        case VAL_OBJ:
            return objectWrite(value, buffer, size);
    }
}

int valuePrint(Value value) {
    return valueWrite(value, NULL, 0);
}

OptValue valueGetField(Value obj, ObjString *name) {
    if (!IS_OBJ(obj))
        reportRuntimeError("%s does not have fields", decodeValueType(obj));
    return objectGetField(obj, name);
}

void valueSetField(Value obj, ObjString *name, Value value) {
    if (!IS_OBJ(obj))
        reportRuntimeError("%s does not have fields", decodeValueType(obj));
    objectSetField(obj, name, value);
}

Value valueGetAt(Value obj, Value key) {
    if (!IS_OBJ(obj))
        reportRuntimeError("%s is not subscriptalbe", decodeValueType(obj));
    return objectGetAt(obj, key);
}

void valueSetAt(Value obj, Value key, Value value) {
    if (!IS_OBJ(obj))
        reportRuntimeError("%s does not support item assignement", decodeValueType(obj));
    objectSetAt(obj, key, value);
}

ObjString *valueToStr(Value value) {
    char buffer[128];
    int length = valueWrite(value, buffer, sizeof(buffer));
    ObjString *string = copyString(buffer, length);
    return string;
}

const char* decodeValueType(Value value) {
    switch (value.type) {
        case VAL_BOOL:
            return "<type bool>";
        case VAL_NIL:
            return "<type nil>";
        case VAL_NUMBER:
            return "<type number>";
        case VAL_OBJ:
            return decodeObjType(value);
    }
    return ""; // uncreachable
}

const char *decodeValueTypeClean(Value value) {
    switch (value.type) {
        case VAL_BOOL:
            return "bool";
        case VAL_NIL:
            return "nil";
        case VAL_NUMBER:
            return "number";
        case VAL_OBJ:
            return decodeObjTypeClean(value);
    }
    return ""; // uncreachable
}