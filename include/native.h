#ifndef NATIVE_H
#define NATIVE_H

#include "value.h"

Value printNative(int argc, Value *argv);

Value clockNative(int argc, Value *argv);

Value sqrtNative(int argc, Value *argv);

Value minNative(int argc, Value *argv);

Value maxNative(int argc, Value *argv);

#endif