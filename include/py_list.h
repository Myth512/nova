#ifndef PY_LIST_H
#define PY_LIST_H 

#include "value.h"

Value PyList_Append(int argc, Value *argv);

Value PyList_Pop(int argc, Value *argv);

Value PyList_Insert(int argc, Value *argv);

Value PyList_Reverse(int argc, Value *argv);

Value PyList_Sort(int argc, Value *argv);

Value PyList_Equal(int argc, Value *argv);

Value PyList_NotEqual(int argc, Value *argv);

Value PyList_Greater(int argc, Value *argv);

Value PyList_GreaterEqual(int argc, Value *argv);

Value PyList_Less(int argc, Value *argv);

Value PyList_LessEqual(int argc, Value *argv);

Value PyList_Add(int argc, Value *argv);

Value PyList_Multiply(int argc, Value *argv);

Value PyList_GetAt(int argc, Value *argv);

Value PyList_SetAt(int argc, Value *argv);

Value PyList_Len(int argc, Value *argv);

#endif