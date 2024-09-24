#include "value.h"

void initValueVec(ValueVec *vec) {
    vec->size = 0;
    vec->capacity = 0;
    vec->values = NULL;
}

void freeValueVec(ValueVec *vec) {
    FREE_VEC(Value, vec->values, vec->capacity);
    initValueVec(vec);
}

void pushValue(ValueVec *vec, Value value) {
    if (vec->size + 1 >= vec->capacity) {
        int oldCapacity = vec->capacity;
        vec->capacity = GROW_CAPACITY(oldCapacity);
        vec->values = GROW_VEC(Value, vec->values, oldCapacity, vec->capacity);
    }

    vec->values[vec->size++] = value;
}

void printValue(Value value) {
    switch (value.type) {
        case VAL_BOOL:
            printf(AS_BOOL(value) ? "true" : "false");
            break;
        case VAL_NIL:
            printf("nil");
            break;
        case VAL_NUMBER:
            printf("%g", AS_NUMBER(value));
            break;
    }
}