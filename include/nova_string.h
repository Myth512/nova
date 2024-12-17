#ifndef NOVA_STRING_H
#define NOVA_STRING_H

#include "value.h"

Value novaStringReverse(int argc, Value *argv);

Value novaStringReplace(int argc, Value *argv);

Value novaStringEqual(int argc, Value *argv);

Value novaStringNotEqual(int argc, Value *argv);

Value novaStringGreater(int argc, Value *argv);

Value novaStringGreaterEqual(int argc, Value *argv);

Value novaStringLess(int argc, Value *argv);

Value novaStringLessEqual(int argc, Value *argv);

Value novaStringAdd(int argc, Value *argv);

Value novaStringMultiply(int argc, Value *argv);

Value novaStringGetAt(int argc, Value *argv);

Value novaStringLen(int argc, Value *argv);

Value novaStringHash(int argc, Value *argv);

#endif