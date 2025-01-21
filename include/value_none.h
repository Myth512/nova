#ifndef VALUE_NONE_H
#define VALUE_NONE_H

#include "value.h"

#define NONE_METHODS (ValueMethods) { \
    .eq = None_Equal,                 \
    .ne = None_NotEqual,              \
    .hash = None_Hash,                \
    .toBool = None_ToBool,            \
    .getattr = None_GetAttribute,     \
    .str = None_ToStr,                \
    .repr = None_ToStr                \
}

Value None_Equal(Value a, Value b);

Value None_NotEqual(Value a, Value b);

Value None_GetAttribute(Value value, ObjString *name);

uint64_t None_Hash(Value value);

Value None_Class(Value value);

bool None_ToBool(Value value);

int None_ToStr(Value value, char *buffer, size_t size);

#endif