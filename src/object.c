#include <string.h>

#include "object.h"
#include "vm.h"
#include "memory.h"
#include "value.h"
#include "table.h"

Obj* allocateObject(size_t size, ObjType type) {
    Obj *object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;
    object->next = vm.objects;
    vm.objects = object;
    return object;
}

static char convertToEscapeChar(char c) {
    switch (c) {
        case 'a':
            return '\a';
        case 'b':
            return '\n';
        case 'f':
            return '\f';
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        case 'v':
            return '\v';
        case '0':
            return '\0';
        case 'e':
            return '\e';
        default:
            return c;
    }
}

int resolveEscapeSequence(const char *source, int sourceLength, char *destination) {
    const char *sourceChar = source;
    const char *endChar = source + sourceLength;
    char *destinationChar = destination;
    int destinationLength = 0;
    while (sourceChar < endChar) {
        if (*sourceChar != '\\') {
            *destinationChar = *sourceChar;
            sourceChar++;
            destinationChar++;
            destinationLength++;
        } else {
            sourceChar++;
            *destinationChar = convertToEscapeChar(*sourceChar);
            sourceChar++;
            destinationChar++;
            destinationLength++;
        }
    }
    *destinationChar = '\0';
    return destinationLength;
}

static uint32_t hashString(const char *key, int length) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

uint32_t getHash(ObjString *string) {
    if (!string->isHashed) {
        string->hash = hashString(string->chars, string->length);
        string->isHashed = true;
    }
    return string->hash;
}

static ObjString* allocateString(const char *chars, int length) {
    int size = sizeof(ObjString) + length + 1;
    ObjString *string = (ObjString*)allocateObject(size, OBJ_STRING); 
    string->isInterned = false;
    string->isHashed = false;

    string->length = resolveEscapeSequence(chars, length, string->chars);
    int newSize = size - (length - string->length);
    reallocate(string, size, newSize);

    return string;
}

ObjString* copyString(const char *chars, int length) {
    return allocateString(chars, length);
}

ObjString* takeString(char *chars, int length) {
    // uint32_t hash = hashString(chars, length);
    // ObjString *interned = tableFindString(&vm.strings, chars, length, hash);

    // if (interned != NULL) {
    //     FREE_VEC(char, chars, length + 1);
    //     return interned;
    // }

    return allocateString(chars, length);
}

ObjRawString* createRawString(const char *chars, int length) {
    ObjRawString *string = (ObjRawString*)allocateObject(sizeof(ObjRawString), OBJ_RAW_STRING);
    string->chars = chars;
    string->length = length;
    return string;
}

bool compareStrings(ObjString *a, ObjString *b) {
    if (a->isInterned && b->isInterned)
        return a == b;
    if (a->length != b->length) 
        return false;
    return memcmp(a->chars, b->chars, a->length) == 0;
}

ObjFunction* createFunction() {
    ObjFunction *function = (ObjFunction*)allocateObject(sizeof(ObjFunction), OBJ_FUNCTION);
    function->arity = 0;
    function->name = NULL;
    initCodeVec(&function->code);
    return function;
}

ObjNative* createNative(NativeFn function, const char *name) {
    ObjNative *native = (ObjNative*)allocateObject(sizeof(ObjNative), OBJ_NATIVE);
    native->function = function;
    native->name = name;
    return native;
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_FUNCTION:
            ObjFunction *function = AS_FUNCTION(value);
            if (function->name == NULL) {
                printf("<script>");
                break;
            }
            printf("<function %s>", function->name->chars);
            break;
        case OBJ_NATIVE:
            printf("<native function %s>", AS_NATIVE(value)->name);
            break;
        case OBJ_STRING:
            printf("%s", AS_CSTRING(value));
            break;
        case OBJ_RAW_STRING:
            ObjRawString *string = AS_RAW_STRING(value);
            for (int i = 0; i < string->length; i++)
                putchar(string->chars[i]);
    }
}

int writeObject(Value value, char *buffer, const size_t maxSize) {
    switch (OBJ_TYPE(value)) {
        case OBJ_FUNCTION:
            ObjFunction *function = AS_FUNCTION(value);
            return snprintf(buffer, maxSize, "<function %s>", function->name->chars);
        case OBJ_NATIVE:
            return snprintf(buffer, maxSize, "<native function %s>", AS_NATIVE(value)->name);
        case OBJ_STRING:
            return snprintf(buffer, maxSize, "%s", AS_CSTRING(value));
        case OBJ_RAW_STRING:
            return resolveEscapeSequence(AS_RAW_STRING(value)->chars, AS_RAW_STRING(value)->length, buffer);
    }
    return -1; // unreachable
}
