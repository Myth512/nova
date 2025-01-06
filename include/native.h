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

Value novaAddr(int argc, Value *argv);

Value Py_Len(int argc, int kwargc);

Value novaBool(int argc, Value *argv);

Value novaInt(int argc, Value *argv);

Value novaFloat(int argc, Value *argv);

Value novaStr(int argc, Value *argv);

Value novaInput(int argc, Value *argv);

#endif