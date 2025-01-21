#ifndef PY_NONE_H
#define PY_NONE_H

#include "value.h"

Value PyNone_Equal(int argc, int kwargc);

Value PyNone_NotEqual(int argc, int kwargc);

Value PyNone_GetAttribute(int argc, int kwargc);

Value PyNone_Hash(int argc, int kwargc);

Value PyNone_ToBool(int argc, int kwargc);

Value PyNone_Class(Value self);

#endif