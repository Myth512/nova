#ifndef VALUE_VEC_H
#define VALUE_VEC_H

#include "value.h"

typedef struct {
    int size;
    int capacity;
    Value *values;
} ValueVec;

void initValueVec(ValueVec *values);

void freeValueVec(ValueVec *values);

void growValueVec(ValueVec *vec);

void pushValue(ValueVec *values, Value value);

Value popValue(ValueVec *vec);

void insertValue(ValueVec *vec, int index, Value value);

void reverseValueVec(ValueVec *vec);

#endif