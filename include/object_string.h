#ifndef OBJECT_STRING_H
#define OBJECT_STRING_H

#include "object.h"

#define STRING_VAL(string)      ((Value){.type=VAL_STRING, .as.object=(Obj*)string})

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

Value String_Equal(Value a, Value b);

Value String_NotEqual(Value a, Value b);

Value String_Greater(Value, Value b);

Value String_GreaterEqual(Value a, Value b);

Value String_Less(Value a, Value b);

Value String_LessEqual(Value a, Value b);

Value String_Add(Value a, Value b);

Value String_Multiply(Value a, Value b);

OptValue String_GetField(Value string, ObjString *name);

Value String_GetAt(ObjString *string, Value index);

uint64_t String_Hash(Value value);

long long String_Len(Value string);

bool String_ToBool(Value string);

long long String_ToInt(Value string);

double String_ToFloat(Value string);

int String_ToStr(Value value, char *buffer, size_t size);

#endif