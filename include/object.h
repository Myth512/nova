#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>

#include "value.h"
#include "table.h"
#include "code.h"

#define OBJ_TYPE(value)         (AS_OBJ(value)->type)

#define IS_CLOSURE(value)       isObjType(value, OBJ_CLOSURE)
#define IS_FUNCTION(value)      isObjType(value, OBJ_FUNCTION)
#define IS_NATIVE(value)        isObjType(value, OBJ_NATIVE)
#define IS_STRING(value)        isObjType(value, OBJ_STRING)
#define IS_ARRAY(value)         isObjType(value, OBJ_ARRAY)
#define IS_CLASS(value)         isObjType(value, OBJ_CLASS)
#define IS_INSTANCE(value)      isObjType(value, OBJ_INSTANCE)
#define IS_METHOD(value)        isObjType(value, OBJ_METHOD)
#define IS_NATIVE_METHOD(value) isObjType(value, OBJ_NATIVE_METHOD)

#define AS_CLOSURE(value)       ((ObjClosure*)AS_OBJ(value))
#define AS_FUNCTION(value)      ((ObjFunction*)AS_OBJ(value))
#define AS_NATIVE(value)        ((ObjNative*)AS_OBJ(value))
#define AS_STRING(value)        ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)       (((ObjString*)AS_OBJ(value))->chars)
#define AS_RAW_STRING(value)    ((ObjRawString*)AS_OBJ(value))
#define AS_ARRAY(value)         ((ObjArray*)AS_OBJ(value))
#define AS_UPVALUE(value)       ((ObjUpvalue*)AS_OBJ(value))
#define AS_CLASS(value)         ((ObjClass*)AS_OBJ(value))
#define AS_INSTANCE(value)      ((ObjInstance*)AS_OBJ(value))
#define AS_METHOD(value)        ((ObjMethod*)AS_OBJ(value))
#define AS_NATIVE_METHOD(value) ((ObjNativeMethod*)AS_OBJ(value))

typedef enum {
    OBJ_CLASS,
    OBJ_METHOD,
    OBJ_NATIVE_METHOD,
    OBJ_INSTANCE,
    OBJ_CLOSURE,
    OBJ_FUNCTION,
    OBJ_NATIVE,
    OBJ_STRING,
    OBJ_RAW_STRING,
    OBJ_UPVALUE,
    OBJ_ARRAY
} ObjType;

struct Obj {
    ObjType type;
    bool isMarked;
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
    Value closed;
    struct ObjUpvalue *next;
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

typedef struct {
    Obj obj;
    ValueVec values;
} ObjArray;

typedef struct {
    Obj obj;
    ObjString *name;
    Table methods;
} ObjClass;

typedef struct {
    Obj obj;
    ObjClass *class;
    Table fields;
} ObjInstance;

typedef struct {
    Obj obj;
    Value reciever;
    ObjClosure *method;
} ObjMethod;

typedef struct {
    Obj obj;
    Value reciever;
    char *name;
    NativeFn method;
} ObjNativeMethod;

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

Obj* allocateObject(size_t size, ObjType type);

ObjRawString* createRawString(const char *chars, int length);

ObjUpvalue *createUpvalue(Value *slot);

ObjFunction* createFunction();

ObjClosure* createClosure(ObjFunction *function);

ObjNative* createNative(NativeFn function, const char *name);

ObjClass *createClass(ObjString *name);

ObjInstance *createInstance(ObjClass *class);

ObjMethod *createMethod(Value reciever, ObjClosure *method);

ObjNativeMethod *createNativeMethod(Value reciever, NativeFn function, const char *name);

ObjString* copyString(const char *chars, int length);

ObjString* allocateString(const char *chars, int length);

ObjString* takeString(char *chars, int length);

bool compareStrings(ObjString *a, ObjString *b);

uint32_t getHash(ObjString *string);

ObjArray* allocateArray(int size);

int resolveEscapeSequence(const char *source, int sourceLength, char *destination);

void printObject(Value value);

int writeObject(Value value, char *buffer, const size_t maxSize);

const char* decodeObjType(Value value);

uint64_t hashObject(Value value);

#endif