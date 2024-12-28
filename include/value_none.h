#ifndef VALUE_NONE_H
#define VALUE_NONE_H

#include "value.h"

Value noneEqual(Value a, Value b);

Value noneNotEqual(Value a, Value b);

uint64_t noneHash(Value value);

bool noneToBool(Value value);

long long noneToInt(Value value);

double noneToFloat(Value value);

#endif