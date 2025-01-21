#ifndef PY_FLOAT_H
#define PY_FLOAT_H

#include "value.h"

Value PyFloat_Equal(int argc, int kwargc);

Value PyFloat_NotEqual(int argc, int kwargc);

Value PyFloat_Greater(int argc, int kwargc);

Value PyFloat_GreaterEqual(int argc, int kwargc);

Value PyFloat_Less(int argc, int kwargc);

Value PyFloat_LessEqual(int argc, int kwargc);

Value PyFloat_Add(int argc, int kwargc);

Value PyFloat_Subtract(int argc, int kwargc);

Value PyFloat_RightSubtract(int argc, int kwargc);

Value PyFloat_Multiply(int argc, int kwargc);

Value PyFloat_TrueDivide(int argc, int kwargc);

Value PyFloat_RightTrueDivide(int argc, int kwargc);

Value PyFloat_FloorDivide(int argc, int kwargc);

Value PyFloat_RightFloorDivide(int argc, int kwargc);

Value PyFloat_Modulo(int argc, int kwargc);

Value PyFloat_RightModulo(int argc, int kwargc);

Value PyFloat_Power(int argc, int kwargc);

Value PyFloat_RightPower(int argc, int kwargc);

Value PyFloat_Positive(int argc, int kwargc);

Value PyFloat_Negative(int argc, int kwargc);

Value PyFloat_Class(Value value);

Value PyFloat_Init(Value callee, int argc, Value *argv);

Value PyFloat_GetAttribute(int argc, int kwargc);

Value PyFloat_IsInteger(int argc, int kwargc);

Value PyFloat_Hash(Value value);

Value PyFloat_ToBool(Value value);

Value PyFloat_ToInt(Value value);

Value PyFloat_ToFloat(Value value);

#endif