#ifndef VALUE_H
#define VALUE_H

#include <stdlib.h>

#include "memory.h"

typedef double Value;

typedef struct {
    int size;
    int capacity;
    Value *values;
} ValueVec;

void initValueVec(ValueVec *values);

void freeValueVec(ValueVec *values);

void pushValue(ValueVec *values, Value value);

void printValue(Value value);

#endif