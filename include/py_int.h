#ifndef PY_INT_H
#define PY_INT_H

#include "value.h"

Value PyInt_Equal(int argc, int kwargc);

Value PyInt_NotEqual(int argc, int kwargc);

Value PyInt_Greater(int argc, int kwargc);

Value PyInt_GreaterEqual(int argc, int kwargc);

Value PyInt_Less(int argc, int kwargc);

Value PyInt_LessEqual(int argc, int kwargc);

Value PyInt_Add(int argc, int kwargc);

Value PyInt_Subtract(int argc, int kwargc);

Value PyInt_RightSubtract(int argc, int kwargc);

Value PyInt_Multiply(int argc, int kwargc);

Value PyInt_TrueDivide(int argc, int kwargc);

Value PyInt_RightTrueDivide(int argc, int kwargc);

Value PyInt_FloorDivide(int argc, int kwargc);

Value PyInt_RightFloorDivide(int argc, int kwargc);

Value PyInt_Modulo(int argc, int kwargc);

Value PyInt_RightModulo(int argc, int kwargc);

Value PyInt_Power(int argc, int kwargc);

Value PyInt_RightPower(int argc, int kwargc);

Value PyInt_Positive(int argc, int kwargc);

Value PyInt_Negative(int argc, int kwargc);

Value PyInt_And(int argc, int kwargc);

Value PyInt_Xor(int argc, int kwargc);

Value PyInt_Or(int argc, int kwargc);

Value PyInt_Invert(int argc, int kwargc);

Value PyInt_LeftShift(int argc, int kwargc);

Value PyInt_RightLeftShift(int argc, int kwargc);

Value PyInt_RightShift(int argc, int kwargc);

Value PyInt_RightRightShift(int argc, int kwargc);

Value PyInt_GetAttribute(int argc, int kwargc);

Value PyInt_Class(Value self);

#endif