#ifndef PY_DICT_H
#define PY_DICT_H

#include "value.h"

Value PyDict_Clear(int argc, int kwargc);

Value PyDict_Copy(int argc, int kwargc);

Value PyDict_FromKeys(int argc, int kwargc);

Value PyDict_Get(int argc, int kwargc);

Value PyDict_Items(int argc, int kwargc);

Value PyDict_Keys(int argc, int kwargc);

Value PyDict_Pop(int argc, int kwargc);

Value PyDict_PopItem(int argc, int kwargc);

Value PyDict_SetDefault(int argc, int kwargc);

Value PyDict_Update(int argc, int kwargc);

Value PyDict_Values(int argc, int kwargc);

#endif