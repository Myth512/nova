#ifndef STRING_H
#define STRING_H

#include "value.h"

Value stringReverse(int argc, Value *argv);

Value stringReplace(int argc, Value *argv);

Value stringEqual(int argc, Value *argv);

Value stringNotEqual(int argc, Value *argv);

Value stringGreater(int argc, Value *argv);

Value stringGreaterEqual(int argc, Value *argv);

Value stringLess(int argc, Value *argv);

Value stringLessEqual(int argc, Value *argv);

Value stringAdd(int argc, Value *argv);

Value stringMultiply(int argc, Value *argv);

Value stringGetAt(int argc, Value *argv);

Value stringLen(int argc, Value *argv);

Value stringHash(int argc, Value *argv);

#endif