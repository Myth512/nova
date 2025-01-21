#ifndef PY_BOOL_H
#define PY_BOOL_H

#include "value.h"

Value PyBool_Equal(int argc, int kwargc);

Value PyBool_NotEqual(int argc, int kwargc);

Value PyBool_Greater(int argc, int kwargc);

Value PyBool_GreaterEqual(int argc, int kwargc);

Value PyBool_Less(int argc, int kwargc);

Value PyBool_LessEqual(int argc, int kwargc);

Value PyBool_Add(int argc, int kwargc);

Value PyBool_Subtract(int argc, int kwargc);

Value PyBool_RightSubtract(int argc, int kwargc);

Value PyBool_Multiply(int argc, int kwargc);

Value PyBool_TrueDivide(int argc, int kwargc);

Value PyBool_RightTrueDivide(int argc, int kwargc);

Value PyBool_FloorDivide(int argc, int kwargc);

Value PyBool_RightFloorDivide(int argc, int kwargc);

Value PyBool_Modulo(int argc, int kwargc);

Value PyBool_RightModulo(int argc, int kwargc);

Value PyBool_Power(int argc, int kwargc);

Value PyBool_RightPower(int argc, int kwargc);

Value PyBool_Positive(int argc, int kwargc);

Value PyBool_Negative(int argc, int kwargc);

Value PyBool_And(int argc, int kwargc);

Value PyBool_Xor(int argc, int kwargc);

Value PyBool_Or(int argc, int kwargc);

Value PyBool_Invert(int argc, int kwargc);

Value PyBool_LeftShift(int argc, int kwargc);

Value PyBool_RightLeftShift(int argc, int kwargc);

Value PyBool_RightShift(int argc, int kwargc);

Value PyBool_RightRightShift(int argc, int kwargc);

Value PyBool_GetAttribute(int argc, int kwargc);

Value PyBool_Class(Value self);

#endif