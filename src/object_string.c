#include <string.h>

#include "object_string.h"

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
    return !stringsEqual(a, b);
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