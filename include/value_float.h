
#ifndef VALUE_FLOAT_H
#define VALUE_FLOAT_H

#include "value.h"

#define FLOAT_VAL(value)    ((Value){VAL_FLOAT, {.floating=value}})

#define IS_FLOAT(value)     ((value).type == VAL_FLOAT)

#define AS_FLOAT(value)     ((value).as.floating)

Value floatEqual(Value a, Value b);

Value floatNotEqual(Value a, Value b);

Value floatGreater(Value a, Value b);

Value floatGreaterEqual(Value a, Value b);

Value floatLess(Value a, Value b);

Value floatLessEqual(Value a, Value b);

Value floatAdd(Value a, Value b);

Value floatSubtract(Value a, Value b);

Value floatMultiply(Value a, Value b);

Value floatTrueDivide(Value a, Value b);

Value floatFloorDivide(Value a, Value b);

Value floatModulo(Value a, Value b);

Value floatPower(Value a, Value b);

Value floatPositive(Value a);

Value floatNegative(Value a);

uint64_t floatHash(Value value);

bool floatToBool(Value value);

long long floatToInt(Value value);

double floatToFloat(Value value);

int floatToStr(Value value, char *buffer, const size_t size);

#endif