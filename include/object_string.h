#ifndef OBJECT_STRING_H
#define OBJECT_STRING_H

#include "object.h"

#define IS_STRING(value)        isObjType(value, OBJ_STRING)

#define AS_STRING(value)        ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)       (((ObjString*)AS_OBJ(value))->chars)
#define AS_RAW_STRING(value)    ((ObjRawString*)AS_OBJ(value))

struct ObjString {
    Obj obj;
    int length;
    uint32_t hash;
    bool isHashed;
    bool isInterned;
    char chars[];
};

struct ObjRawString {
    Obj obj;
    int length;
    const char *chars;
};

ObjString *allocateString(size_t length);

ObjString *copyString(const char *chars, size_t length);

ObjString *copyEscapedString(const char *chars, size_t length);

ObjRawString* createRawString(const char *chars, int length);

bool stringEqual(ObjString *a, ObjString *b);

bool stringNotEqual(ObjString *a, ObjString *b);

bool stringGreater(ObjString *a, ObjString *b);

bool stringGreaterEqual(ObjString *a, ObjString *b);

bool stringLess(ObjString *a, ObjString *b);

bool stringLessEqual(ObjString *a, ObjString *b);

ObjString *stringAdd(ObjString *a, ObjString *b);

ObjString *stringMultiply(ObjString *string, int scalar);

int writeRawstring(char *buffer, ObjRawString *string);

uint32_t getStringHash(ObjString *string);

#endif