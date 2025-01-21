#ifndef PY_STRING_H
#define PY_STRING_H

#include "object.h"

Value PyString_Equal(int argc, int kwargc);

Value PyString_NotEqual(int argc, int kwargc);

Value PyString_Greater(int argc, int kwargc);

Value PyString_GreaterEqual(int argc, int kwargc);

Value PyString_Less(int argc, int kwargc);

Value PyString_LessEqual(int argc, int kwargc);

Value PyString_Add(int argc, int kwargc);

Value PyString_Multiply(int argc, int kwargc);

Value PyString_RightMultiply(int argc, int kwargc);

Value PyString_Contains(int argc, int kwargc);

Value PyString_Iter(int argc, int kwargc);

Value PyString_GetAttribute(int argc, int kwargc);

Value PyString_GetItem(int argc, int kwargc);

Value PyString_Hash(int argc, int kwargc);

Value PyString_Len(int argc, int kwargc);

Value PyString_Capitalize(int argc, int kwargc);

Value PyString_CaseFold(int argc, int kwargc);

Value PyString_Center(int argc, int kwargc);

Value PyString_Count(int argc, int kwargc);

Value PyString_Encode(int argc, int kwargc);

Value PyString_Endswith(int argc, int kwargc);

Value PyString_Expandtabs(int argc, int kwargc);

Value PyString_Find(int argc, int kwargc);

Value PyString_Format(int argc, int kwargc);

Value PyString_FormatMap(int argc, int kwargc);

Value PyString_Index(int argc, int kwargc);

Value PyString_IsAlnum(int argc, int kwargc);

Value PyString_IsAlpha(int argc, int kwargc);

Value PyString_IsAscii(int argc, int kwargc);

Value PyString_IsDecimal(int argc, int kwargc);

Value PyString_IsDigit(int argc, int kwargc);

Value PyString_IsIdentifier(int argc, int kwargc);

Value PyString_IsLower(int argc, int kwargc);

Value PyString_IsNumeric(int argc, int kwargc);

Value PyString_IsPrintable(int argc, int kwargc);

Value PyString_IsSpace(int argc, int kwargc);

Value PyString_IsTitle(int argc, int kwargc);

Value PyString_IsUpper(int argc, int kwargc);

Value PyString_Join(int argc, int kwargc);

Value PyString_Ljust(int argc, int kwargc);

Value PyString_Lower(int argc, int kwargc);

Value PyString_Lstrip(int argc, int kwargc);

Value PyString_Maketrans(int argc, int kwargc);

Value PyString_Partition(int argc, int kwargc);

Value PyString_Replace(int argc, int kwargc);

Value PyString_Rfind(int argc, int kwargc);

Value PyString_Rindex(int argc, int kwargc);

Value PyString_Rjust(int argc, int kwargc);

Value PyString_Rpartition(int argc, int kwargc);

Value PyString_Rsplit(int argc, int kwargc);

Value PyString_Rstrip(int argc, int kwargc);

Value PyString_Split(int argc, int kwargc);

Value PyString_Splitlines(int argc, int kwargc);

Value PyString_Startswith(int argc, int kwargc);

Value PyString_Strip(int argc, int kwargc);

Value PyString_Swapcase(int argc, int kwargc);

Value PyString_Title(int argc, int kwargc);

Value PyString_Translate(int argc, int kwargc);

Value PyString_Upper(int argc, int kwargc);

Value PyString_Zfill(int argc, int kwargc);

Value PyString_Class(Value self);

#endif