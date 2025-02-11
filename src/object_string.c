#include <string.h>
#include <stdlib.h>

#include "value.h"
#include "value_int.h"
#include "object_string.h"
#include "object_string_iterator.h"
#include "object_exception.h"
#include "object_class.h"
#include "methods_string.h"
#include "object_slice.h"
#include "vm.h"

ObjString *allocateString(size_t length) {
    size_t size = sizeof(ObjString) + length + 1;
    ObjString *string = (ObjString*)allocateObject(size, VAL_STRING);
    string->chars[length] = '\0';
    string->isInterned = false;
    string->isHashed = false;
    string->length = length;
    return string;
}

ObjString *copyString(const char *chars, size_t length) {
    if (length == 0)
        length = strlen(chars);
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

Value String_RightMultiply(Value a, Value b) {
    return String_Multiply(a, b);
}

Value String_Contains(Value a, Value b) {
    if (!IS_STRING(b))
        return createException(VAL_TYPE_ERROR, "'in <string>' requires string as left operand, not %s", getValueType(b));

    ObjString *s1 = AS_STRING(a);
    ObjString *s2 = AS_STRING(b);

    for (int offset = 0; offset <= s1->length - s2->length; offset++) {
        if (strncmp(s1->chars + offset, s2->chars, s2->length) == 0)
            return BOOL_VAL(true);
    }
    return BOOL_VAL(false);
}

Value String_Class(Value value) {
    return TYPE_CLASS(str);
}

Value String_Init(Value callee, int argc, Value *argv) {
    return OBJ_VAL(valueToStr(argv[0]));
}

Value String_Iter(Value value) {
    return OBJ_VAL(allocateStringIterator(value));
}

Value String_GetAttribute(Value value, ObjString *name) {
    return getStaticAttribute(value, name, in_string_set);
}

Value String_GetItem(Value value, Value key) {
    if (IS_INT(key)) {
        int index = calculateIndex(AS_INT(key), AS_STRING(value)->length);
        if (index < 0)
            return createException(VAL_INDEX_ERROR, "string index out of range");
        
        ObjString *res = copyString(AS_STRING(value)->chars + index, 1);
        return OBJ_VAL(res);
    } else if (IS_SLICE(key)) {
        ParsedSlice slice = parseSlice(AS_SLICE(key), AS_STRING(value)->length);
        if (slice.step == 0)
            return createException(VAL_VALUE_ERROR, "slice step cannot be zero");

        ObjString *res = allocateString(slice.length);

        if (slice.step > 0) {
            for (int i = slice.start, j = 0; i < slice.stop; i += slice.step, j++) {
                res->chars[j] = AS_STRING(value)->chars[i];
            }
        } else {
            for (int i = slice.stop - 1, j = 0; i >= slice.start; i += slice.step, j++) {
                res->chars[j] = AS_STRING(value)->chars[i];
            }
        }
        return OBJ_VAL(res);
    }
    return createException(VAL_TYPE_ERROR, "string indices must be integers, not '%s'", getValueType(key));
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