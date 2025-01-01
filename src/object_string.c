#include <string.h>
#include <stdlib.h>

#include "value.h"
#include "value_int.h"
#include "object_string.h"
#include "object_class.h"
#include "methods_string.h"

ObjString *allocateString(size_t length) {
    size_t size = sizeof(ObjString) + length + 1;
    ObjString *string = (ObjString*)allocateObject(size, VAL_STRING);
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

static bool compareStrings(ObjString *a, ObjString *b) {
    if (a->length != b->length)
        return false;
    if (a->isInterned && b->isInterned)
        return a == b;
    return strcmp(a->chars, b->chars) == 0;
}

Value String_Equal(Value a, Value b) {
    if (IS_STRING(b))
        return BOOL_VAL(compareStrings(AS_STRING(a), AS_STRING(b)));
    return NOT_IMPLEMENTED_VAL;
}

Value String_NotEqual(Value a, Value b) {
    if (IS_STRING(b))
        return BOOL_VAL(!compareStrings(AS_STRING(a), AS_STRING(b)));
    return NOT_IMPLEMENTED_VAL;
}

Value String_Greater(Value a, Value b) {
    if (IS_STRING(b))
        return BOOL_VAL(strcmp(AS_CHARS(a), AS_CHARS(b)) > 0);
    return NOT_IMPLEMENTED_VAL;
}

Value String_GreaterEqual(Value a, Value b) {
    if (IS_STRING(b))
        return BOOL_VAL(strcmp(AS_CHARS(a), AS_CHARS(b)) >= 0);
    return NOT_IMPLEMENTED_VAL;
}

Value String_Less(Value a, Value b) {
    if (IS_STRING(b))
        return BOOL_VAL(strcmp(AS_CHARS(a), AS_CHARS(b)) < 0);
    return NOT_IMPLEMENTED_VAL;
}

Value String_LessEqual(Value a, Value b) {
    if (IS_STRING(b))
        return BOOL_VAL(strcmp(AS_CHARS(a), AS_CHARS(b)) <= 0);
    return NOT_IMPLEMENTED_VAL;
}

Value String_Add(Value a, Value b) {
    if (!IS_STRING(b))
        return NOT_IMPLEMENTED_VAL;
    
    ObjString *s1 = AS_STRING(a);
    ObjString *s2 = AS_STRING(b);

    int length = s1->length + s2->length;

    ObjString *result = allocateString(length);

    memcpy(result->chars, s1->chars, s1->length);
    memcpy(result->chars + s1->length, s2->chars, s2->length);
    result->chars[length] = '\0';

    return STRING_VAL(result);
}

Value String_Multiply(Value a, Value b) {
    if (!IS_INT(b))
        return NOT_IMPLEMENTED_VAL;
    
    ObjString *string = AS_STRING(a);
    long long scalar = AS_INT(b);

    size_t oldLength = string->length;
    size_t newLength = oldLength * scalar;

    ObjString *result = allocateString(newLength);

    for (int i = 0; i < scalar; i++)
        memcpy(result->chars + i * oldLength, string->chars, oldLength);

    return STRING_VAL(result);
}

Value String_GetAttr(Value value, ObjString *name) {
    return getGperfMethod(value, name, in_string_set);
}

Value String_GetItem(Value value, Value key) {
    if (!IS_INT(key))
        reportRuntimeError("Indicies only int");
    int index = calculateIndex(AS_INT(key), AS_STRING(value)->length);
    if (index == -1)
        reportRuntimeError("Index is out of bounds");
    
    return OBJ_VAL(copyString(AS_STRING(value)->chars + index, 1));
}

static uint64_t hashString(const char *value) {
    uint64_t hash = 0;
    while (*value) {
        hash = (hash * 31) + (unsigned char)(*value); 
        value++;  
    }
    return hash;
}

uint64_t String_Hash(Value value) {
    ObjString *string = AS_STRING(value);
    if (!string->isHashed) {
        string->hash = hashString(string->chars);
        string->isHashed = true;
    }
    return string->hash;
}

long long String_Len(Value value) {
    return AS_STRING(value)->length;
}

bool String_ToBool(Value value) {
    return String_Len(value);
}

long long String_ToInt(Value value) {
    return atoi(AS_CHARS(value));
}

double String_ToFloat(Value value) {
    return atof(AS_CHARS(value));
}

int String_ToStr(Value value, char *buffer, size_t size) {
    writeToBuffer(buffer, size, "%s", AS_CHARS(value));
}

int String_ToRepr(Value value, char *buffer, size_t size) {
    writeToBuffer(buffer, size, "'%s'", AS_CHARS(value));
}