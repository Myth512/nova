#ifndef NATIVE_H
#define NATIVE_H

#include "value.h"

Value Py_Print(int argc, int kwargc);

Value Py_Id(int argc, int kwargc);

Value Py_Len(int argc, int kwargc);

Value Py_Repr(int argc, int kwargc);

Value Py_Input(int argc, int kwargc);

Value Py_Hex(int argc, int kwargc);

Value Py_IsInstance(int argc, int kwargc);

Value Py_Iter(int argc, int kwargc);

Value Py_Next(int argc, int kwargc);

#endif