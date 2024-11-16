#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>

#include "value.h"
#include "code.h"

#define OBJ_TYPE(value)         (AS_OBJ(value)->type)

#define IS_CLOSURE(value)       isObjType(value, OBJ_CLOSURE)
#define IS_FUNCTION(value)      isObjType(value, OBJ_FUNCTION)
#define IS_NATIVE(value)        isObjType(value, OBJ_NATIVE)
#define IS_STRING(value)        isObjType(value, OBJ_STRING)

#define AS_CLOSURE(value)       ((ObjClosure*)AS_OBJ(value))
#define AS_FUNCTION(value)      ((ObjFunction*)AS_OBJ(value))
#define AS_NATIVE(value)        ((ObjNative*)AS_OBJ(value))
#define AS_STRING(value)        ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)       (((ObjString*)AS_OBJ(value))->chars)
#define AS_RAW_STRING(value)    ((ObjRawString*)AS_OBJ(value))

typedef enum {
    OBJ_CLOSURE,
    OBJ_FUNCTION,
    OBJ_NATIVE,
    OBJ_STRING,
    OBJ_RAW_STRING,
    OBJ_UPVALUE
} ObjType;

struct Obj {
    ObjType type;
    struct Obj *next;
};

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

typedef struct ObjUpvalue {
    Obj obj;
    Value *location;
} ObjUpvalue;

typedef struct {
    Obj obj;
    int arity;
    int upvalueCount;
    CodeVec code;
    ObjString *name;
} ObjFunction;

typedef struct {
    Obj obj;
    ObjFunction *function;
    ObjUpvalue **upvalues;
    int upvalueCount;
} ObjClosure;

typedef Value (*NativeFn)(int argc, Value *argv);

typedef struct {
    Obj obj;
    char *name; 
    NativeFn function;
} ObjNative;

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

Obj* allocateObject(size_t size, ObjType type);

ObjRawString* createRawString(const char *chars, int length);

ObjUpvalue *createUpvalue(Value *slot);

ObjFunction* createFunction();

ObjClosure* createClosure(ObjFunction *function);

ObjNative* createNative(NativeFn function, const char *name);

ObjString* copyString(const char *chars, int length);

ObjString* takeString(char *chars, int length);

bool compareStrings(ObjString *a, ObjString *b);

uint32_t getHash(ObjString *string);

int resolveEscapeSequence(const char *source, int sourceLength, char *destination);

void printObject(Value value);

int writeObject(Value value, char *buffer, const size_t maxSize);

#endif