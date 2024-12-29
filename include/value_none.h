#ifndef VALUE_NONE_H
#define VALUE_NONE_H

#include "value.h"

Value None_Equal(Value a, Value b);

Value None_NotEqual(Value a, Value b);

uint64_t None_Hash(Value value);

bool None_ToBool(Value value);

long long None_ToInt(Value value);

double None_ToFloat(Value value);

int None_ToStr(Value value, char *buffer, size_t size);

#endif