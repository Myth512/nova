#ifndef OBJECT_STRING_H
#define OBJECT_STRING_H

#include "object.h"

#define STRING_VAL(string)      ((Value){.type=VAL_STRING, .as.object=string})

#define IS_STRING(value)        ((value).type == VAL_STRING) 

#define AS_STRING(value)        ((ObjString*)((value).as.object))
#define AS_CHARS(value)         (((ObjString*)((value).as.object))->chars)

struct ObjString {
    Obj obj;
    int length;
    uint64_t hash;
    bool isHashed;
    bool isInterned;
    char chars[];
};

ObjString *allocateString(size_t length);

ObjString *copyString(const char *chars, size_t length);

ObjString *copyEscapedString(const char *chars, size_t length);

Value stringEqual(Value a, Value b);

Value stringNotEqual(Value a, Value b);

Value stringGreater(Value, Value b);

Value stringGreaterEqual(Value a, Value b);

Value stringLess(Value a, Value b);

Value stringLessEqual(Value a, Value b);

Value stringAdd(Value a, Value b);

Value stringMultiply(Value a, Value b);

OptValue stringGetField(Value string, ObjString *name);

Value stringGetAt(ObjString *string, Value index);

uint64_t stringHash(Value value);

int stringLen(ObjString *string);

bool stringToBool(ObjString *string);

int stringToInt(ObjString *string);

double stringToFloat(ObjString *string);

#endif