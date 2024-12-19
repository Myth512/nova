#include <string.h>
#include <stdlib.h>

#include "object_string.h"
#include "string_methods.h"
#include "object_class.h"
#include "vm.h"

ObjString *allocateString(size_t length) {
    size_t size = sizeof(ObjString) + length + 1;
    ObjString *string = (ObjString*)allocateObject(size, OBJ_STRING);
    string->isInterned = false;
    string->isHashed = false;
    string->length = length;
    return string;
}

ObjString *copyString(const char *chars, size_t length) {
    ObjString *string = allocateString(length);
    memcpy(string->chars, chars, length);
    string->chars[string->length] = '\0';
    return string;
}

static char convertToEscapeChar(char c) {
    switch (c) {
        case 'a':
            return '\a';
        case 'b':
            return '\b';
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

static int resolveEscapeSequence(const char *source, int sourceLength, char *destination) {
    const char *sourceChar = source;
    const char *endChar = source + sourceLength;
    char *destinationChar = destination;
    int destinationLength = 0;
    while (sourceChar < endChar) {
        if (*sourceChar != '\\') {
            *destinationChar = *sourceChar;
        } else {
            sourceChar++;
            *destinationChar = convertToEscapeChar(*sourceChar);
        }
        sourceChar++;
        destinationChar++;
        destinationLength++;
    }
    *destinationChar = '\0';
    return destinationLength;
}

ObjString *copyEscapedString(const char *chars, size_t length) {
    ObjString *string = allocateString(length);
    string->length = resolveEscapeSequence(chars, length, string->chars);
    return string;
}

ObjRawString* createRawString(const char *chars, int length) {
    ObjRawString *string = (ObjRawString*)allocateObject(sizeof(ObjRawString), OBJ_RAW_STRING);
    string->chars = chars;
    string->length = length;
    return string;
}

int writeRawstring(char *buffer, ObjRawString *string) {
    return resolveEscapeSequence(string->chars, string->length, buffer);
}

static uint64_t hashString(const char *value) {
    uint64_t hash = 0;
    while (*value) {
        hash = (hash * 31) + (unsigned char)(*value); 
        value++;  
    }
    return hash;
}

uint32_t getStringHash(ObjString *string) {
    if (!string->isHashed) {
        string->hash = hashString(string->chars);
        string->isHashed = true;
    }
    return string->hash;
}

bool stringEqual(ObjString *a, ObjString *b) {
    if (a->length != b->length)
        return false;
    if (a->isInterned && b->isInterned)
        return a == b;
    return strcmp(a->chars, b->chars) == 0;
}

bool stringNotEqual(ObjString *a, ObjString *b) {
    return !stringEqual(a, b);
}

bool stringGreater(ObjString *a, ObjString *b) {
    return strcmp(a->chars, b->chars) > 0;
}

bool stringGreaterEqual(ObjString *a, ObjString *b) {
    return strcmp(a->chars, b->chars) >= 0;
}

bool stringLess(ObjString *a, ObjString *b) {
    return strcmp(a->chars, b->chars) < 0;
}

bool stringLessEqual(ObjString *a, ObjString *b) {
    return strcmp(a->chars, b->chars) <= 0;
}

ObjString *stringAdd(ObjString *a, ObjString *b) {
    int length = a->length + b->length;

    ObjString *result = allocateString(length);

    memcpy(result->chars, a->chars, a->length);
    memcpy(result->chars + a->length, b->chars, b->length);
    result->chars[length] = '\0';

    return result;
}

ObjString *stringMultiply(ObjString *string, int scalar) {
    size_t oldLength = string->length;
    size_t newLength = oldLength * scalar;

    ObjString *result = allocateString(newLength);

    for (int i = 0; i < scalar; i++)
        memcpy(result->chars + i * oldLength, string->chars, oldLength);

    return result;
}

OptValue stringGetField(Value string, ObjString *name) {
    const struct StringMethod *result = in_string_set(name->chars, name->length);
    if (result) {
        ObjNativeMethod *method = createNativeMethod(string, result->method, result->name);
        return (OptValue){.hasValue=true, .value=OBJ_VAL(method)};
    } 
    return (OptValue){.hasValue=false};
}

Value stringGetAt(ObjString *string, Value index) {
    if (!isInt(index))
        reportRuntimeError("Index must be integer number");
    
    int i = asInt(index);
    
    int length = string->length; 
    if (i >= length || i < -length)
        reportRuntimeError("Index is out of range");
    if (i < 0)
        i += length;
    
    const char chr = string->chars[i];
    ObjString *result = copyString(&chr, 1);

    return OBJ_VAL(result);
}

int stringLen(ObjString *string) {
    return string->length;
}

bool stringToBool(ObjString *string) {
    return (bool)stringLen(string);
}

int stringToInt(ObjString *string) {
    return atoi(string->chars);
}

double stringToFloat(ObjString *string) {
    return atof(string->chars);
}