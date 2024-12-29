#ifndef PY_LIST_H
#define PY_LIST_H 

#include "value.h"

Value novaArrayPush(int argc, Value *argv);

Value novaArrayPop(int argc, Value *argv);

Value novaArrayInsert(int argc, Value *argv);

Value novaArrayReverse(int argc, Value *argv);

Value novaArraySort(int argc, Value *argv);

Value novaArrayEqual(int argc, Value *argv);

Value novaArrayNotEqual(int argc, Value *argv);

Value novaArrayGreater(int argc, Value *argv);

Value novaArrayGreaterEqual(int argc, Value *argv);

Value novaArrayLess(int argc, Value *argv);

Value novaArrayLessEqual(int argc, Value *argv);

Value novaArrayAdd(int argc, Value *argv);

Value novaArrayMultiply(int argc, Value *argv);

Value novaArrayGetAt(int argc, Value *argv);

Value novaArraySetAt(int argc, Value *argv);

Value novaArrayLen(int argc, Value *argv);

#endif