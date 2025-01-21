#include <string.h>
#include <ctype.h>

#include "py_string.h"
#include "object_string.h"
#include "value_int.h"
#include "value_float.h"
#include "vm.h"    

Value PyString_Equal(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return String_Equal(self, value);
}

Value PyString_NotEqual(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return String_NotEqual(self, value);
}

Value PyString_Greater(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return String_Greater(self, value);
}

Value PyString_GreaterEqual(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return String_GreaterEqual(self, value);
}

Value PyString_Less(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return String_Less(self, value);
}

Value PyString_LessEqual(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return String_LessEqual(self, value);
}

Value PyString_Add(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return String_Add(self, value);
}

Value PyString_Multiply(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return String_Multiply(self, value);
}

Value PyString_RightMultiply(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return String_RightMultiply(self, value);
}

Value PyString_Contains(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return String_Contains(self, value);
}

Value PyString_Iter(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    return String_Iter(self);
}

Value PyString_GetAttribute(int argc, int kwargc) {
    static char *keywords[] = {"self", "name"};
    Value self, name;
    PARSE_ARGS(&self, &name);

    return String_GetAttribute(self, AS_STRING(name));
}

Value PyString_GetItem(int argc, int kwargc) {
    static char *keywords[] = {"self", "key"};
    Value self, key;
    PARSE_ARGS(&self, &key);

    return String_GetItem(self, key);
}

Value PyString_Hash(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    return INT_VAL(String_Hash(self));
}

Value PyString_Len(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    return INT_VAL(String_Len(self));
}

Value PyString_Capitalize(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    ObjString *string = AS_STRING(self);
    ObjString *res = allocateString(string->length);

    if (string->length == 0)
        return OBJ_VAL(res);
    
    res->chars[0] = toupper(string->chars[0]);

    for (int i = 1; i < res->length; i++)
        res->chars[i] = tolower(string->chars[i]);
    
    return OBJ_VAL(res);
}

Value PyString_CaseFold(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Center(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Count(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Encode(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Endswith(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Expandtabs(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Find(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Format(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_FormatMap(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Index(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_IsAlnum(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_IsAlpha(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_IsAscii(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_IsDecimal(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_IsDigit(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_IsIdentifier(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_IsLower(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_IsNumeric(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_IsPrintable(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_IsSpace(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_IsTitle(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_IsUpper(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Join(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Ljust(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Lower(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    ObjString *string = AS_STRING(self);
    ObjString *res = allocateString(string->length);

    for (int i = 0; i < res->length; i++)
        res->chars[i] = tolower(string->chars[i]);
    
    return OBJ_VAL(res);
}

Value PyString_Lstrip(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Maketrans(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Partition(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Replace(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Rfind(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Rindex(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Rjust(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Rpartition(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Rsplit(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Rstrip(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Split(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Splitlines(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Startswith(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Strip(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Swapcase(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Title(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Translate(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Upper(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    ObjString *string = AS_STRING(self);
    ObjString *res = allocateString(string->length);

    for (int i = 0; i < res->length; i++)
        res->chars[i] = toupper(string->chars[i]);
    
    return OBJ_VAL(res);
}

Value PyString_Zfill(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyString_Class(Value self) {
    return String_Class(self);
}