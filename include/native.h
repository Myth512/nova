#ifndef NATIVE_H
#define NATIVE_H

#include "value.h"

Value printNative(int argc, Value *argv);

Value clockNative(int argc, Value *argv);

Value sqrtNative(int argc, Value *argv);

Value minNative(int argc, Value *argv);

Value maxNative(int argc, Value *argv);

Value typeNative(int argc, Value *argv);

Value novaAddr(int argc, Value *argv);

Value novaLen(int argc, Value *argv);

Value novaBool(int argc, Value *argv);

Value novaInt(int argc, Value *argv);

Value novaFloat(int argc, Value *argv);

Value novaStr(int argc, Value *argv);

#endif