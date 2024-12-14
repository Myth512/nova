#ifndef ARRAY_H
#define ARRAY_H

#include "value.h"

Value arrayPush(int argc, Value *argv);

Value arrayPop(int argc, Value *argv);

Value arrayInsert(int argc, Value *argv);

Value arrayReverse(int argc, Value *argv);

Value arraySort(int argc, Value *argv);

Value arrayEqual(int argc, Value *argv);

Value arrayNotEqual(int argc, Value *argv);

Value arrayGreater(int argc, Value *argv);

Value arrayGreaterEqual(int argc, Value *argv);

Value arrayLess(int argc, Value *argv);

Value arrayLessEqual(int argc, Value *argv);

Value arrayAdd(int argc, Value *argv);

Value arrayMultiply(int argc, Value *argv);

Value arrayGetAt(int argc, Value *argv);

Value arraySetAt(int argc, Value *argv);

Value arrayLen(int argc, Value *argv);


#endif