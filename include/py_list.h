#ifndef PY_LIST_H
#define PY_LIST_H

#include "value.h"

Value PyList_Append(int argc, int kwargc);

Value PyList_Clear(int argc, int kwargc);

Value PyList_Copy(int argc, int kwargc);

Value PyList_Count(int argc, int kwargc);

Value PyList_Extend(int argc, int kwargc);

Value PyList_Index(int argc, int kwargc);

Value PyList_Insert(int argc, int kwargc);

Value PyList_Pop(int argc, int kwargc);

Value PyList_Remove(int argc, int kwargc);

Value PyList_Reverse(int argc, int kwargc);

Value PyList_Sort(int argc, int kwargc);

#endif