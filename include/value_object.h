#ifndef VALUE_OBJECT_H
#define VALUE_OBJECT_H

#include "value.h"

Value Object_Equal(Value a, Value b);

Value Object_NotEqual(Value a, Value b);

int Object_ToStr(Value value, char *buffer, const size_t size);

#endif