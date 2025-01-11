#ifndef NATIVE_H
#define NATIVE_H

#include "value.h"

Value printNative(int argc, Value *argv);

Value Py_Time(int argc, Value *argv);

Value Py_Print(int argc, int kwargc);

Value sqrtNative(int argc, Value *argv);

Value minNative(int argc, Value *argv);

Value maxNative(int argc, Value *argv);

Value typeNative(int argc, Value *argv);

Value Py_Id(int argc, int kwargc);

Value Py_Len(int argc, int kwargc);

Value Py_Repr(int argc, int kwargc);

Value Py_Input(int argc, int kwargc);

Value Py_Hex(int argc, int kwargc);

Value Py_IsInstance(int argc, int kwargc);

Value Py_Iter(int argc, int kwargc);

Value Py_Next(int argc, int kwargc);

#endif