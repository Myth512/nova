#ifndef PY_RANGE_H
#define PY_RANGE_H

#include "value.h"

Value PyRange_Start(Value self);

Value PyRange_Step(Value self);

Value PyRange_Stop(Value self);

Value PyRange_Class(Value self);

#endif