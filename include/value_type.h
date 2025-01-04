#ifndef VALUE_TYPE_H
#define VALUE_TYPE_H

#include "value.h"

#define TYPE_METHODS (ValueMethods) { \
    .init = Type_Init,                \
    .class = Type_Class,              \
}

Value Type_Init(Value callee, int argc, Value *argv);

Value Type_Class(Value value);

#endif