#ifndef VALUE_INT_H
#define VALUE_INT_H

#include "value.h"

#define BOOL_VAL(value)     ((Value){VAL_BOOL, {.integer=value}})
#define INT_VAL(value)      ((Value){VAL_INT, {.integer=value}}) 

#define IS_BOOL(value)      ((value).type == VAL_BOOL)
#define IS_INT(value)       ((value).type == VAL_INT)

#define AS_BOOL(value)      ((value).as.integer)
#define AS_INT(value)       ((value).as.integer)

Value intEqual(Value a, Value b);

Value intNotEqual(Value a, Value b);

Value intGreater(Value a, Value b);

Value intGreaterEqual(Value a, Value b);

Value intLess(Value a, Value b);

Value intLessEqual(Value a, Value b);

Value intAdd(Value a, Value b);

Value intSubtract(Value a, Value b);

Value intMultiply(Value a, Value b);

Value intTrueDivide(Value a, Value b);

Value intFloorDivide(Value a, Value b);

Value intModulo(Value a, Value b);

Value intPower(Value a, Value b);

Value intPositive(Value a);

Value intNegative(Value a);

Value intAnd(Value a, Value b); 

Value intXor(Value a, Value b);

Value intOr(Value a, Value b);

Value intInvert(Value a);

Value intLeftShift(Value a, Value b);

Value intRightShift(Value a, Value b);

uint64_t intHash(Value value);

bool intToBool(Value value);

long long intToInt(Value value);

double intToFloat(Value value);

ObjString *intToStr(Value value);

ObjString *intToRepr(Value value);

#endif