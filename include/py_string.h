#ifndef PY_STRING_H
#define PY_STRING_H

#include "value.h"

Value PyString_Reverse(int argc, Value *argv);

Value PyString_Replace(int argc, Value *argv);

Value PyString_Equal(int argc, Value *argv);

Value PyString_NotEqual(int argc, Value *argv);

Value PyString_Greater(int argc, Value *argv);

Value PyString_GreaterEqual(int argc, Value *argv);

Value PyString_Less(int argc, Value *argv);

Value PyString_LessEqual(int argc, Value *argv);

Value PyString_Add(int argc, Value *argv);

Value PyString_Multiply(int argc, Value *argv);

Value PyString_GetAt(int argc, Value *argv);

Value PyString_Len(int argc, Value *argv);

Value PyString_Hash(int argc, Value *argv);

#endif