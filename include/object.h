#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>

#include "value.h"

#define OBJ_TYPE(value)         (AS_OBJ(value)->type)

#define IS_STRING(value)        isObjType(value, OBJ_STRING)

#define AS_STRING(value)        ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)       (((ObjString*)AS_OBJ(value))->chars)
#define AS_RAW_STRING(value)    (((ObjRawString*)AS_OBJ(value)))

typedef enum {
    OBJ_STRING,
    OBJ_RAW_STRING
} ObjType;

struct Obj {
    ObjType type;
    struct Obj *next;
};

struct ObjString {
    Obj obj;
    bool isInterned;
    int length;
    uint32_t hash;
    bool isHashed;
    char chars[];
};

struct ObjRawString {
    Obj obj;
    int length;
    char *chars;
};

Obj* allocateObject(size_t size, ObjType type);

ObjString* copyString(const char *chars, int length);

ObjString* takeString(char *chars, int length);

bool compareStrings(ObjString *a, ObjString *b);

uint32_t getHash(ObjString *string);

int resolveEscapeSequence(const char *source, int sourceLength, char *destination);

void printObject(Value value);

int writeObject(Value value, char *buffer);

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

ObjRawString* createRawString(char *chars, int length);


#endif