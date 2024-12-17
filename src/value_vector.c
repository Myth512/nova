#include "value_vector.h"
#include "memory.h"

void initValueVec(ValueVec *vec) {
    vec->size = 0;
    vec->capacity = 0;
    vec->values = NULL;
}

void freeValueVec(ValueVec *vec) {
    FREE_VEC(Value, vec->values, vec->capacity);
    initValueVec(vec);
}

void growValueVec(ValueVec *vec) {
    int oldCapacity = vec->capacity;
    vec->capacity = GROW_CAPACITY(oldCapacity);
    vec->values = GROW_VEC(Value, vec->values, oldCapacity, vec->capacity);
}

void pushValue(ValueVec *vec, Value value) {
    if (vec->size + 1 >= vec->capacity) {
        growValueVec(vec);
    }

    vec->values[vec->size++] = value;
}

void insertValue(ValueVec *vec, int index, Value value) {
    if (vec->size + 1 >= vec->capacity) {
        growValueVec(vec);
    }

    for (int i = vec->size; i > index; i--) {
        vec->values[i] = vec->values[i-1];
    }
    vec->values[index] = value;
    vec->size++;
}

void reverseValueVec(ValueVec *vec) {
    int size = vec->size;
    for (int i = 0; i < size / 2; i++) {
        Value tmp = vec->values[i];
        vec->values[i] = vec->values[size - i - 1];
        vec->values[size - i - 1] = tmp;
    }
}

Value popValue(ValueVec *vec) {
    return vec->values[--vec->size];
}
