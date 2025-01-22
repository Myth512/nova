#ifndef PY_SLICE_H
#define PY_SLICE_H

#include "value.h"

Value PySlice_Indices(int argc, int kwargc);

Value PySlice_Start(Value self);

Value PySlice_Step(Value self);

Value PySlice_Stop(Value self);

Value PySlice_Class(Value self);

#endif