#include <stdlib.h>

#include "code.h"
#include "memory.h"

void initCodeVec(CodeVec *vec) {
    vec->size = 0;
    vec->capacity = 0;
    vec->code = NULL;
    vec->lines = NULL;
    vec->columns = NULL;
    vec->lengths = NULL;
    initValueVec(&vec->constants);
}

void freeCodeVec(CodeVec *vec) {
    FREE_VEC(uint8_t, vec->code, vec->capacity);
    FREE_VEC(int, vec->lines, vec->capacity);
    FREE_VEC(int, vec->columns, vec->capacity);
    FREE_VEC(int, vec->lengths, vec->capacity);
    freeValueVec(&vec->constants);
    initCodeVec(vec);
}

void pushInstruction(CodeVec *vec, uint8_t byte, int line, int column, int length) {
    if (vec->size + 1 >= vec->capacity) {
        int oldCapacity = vec->capacity;
        vec->capacity = GROW_CAPACITY(oldCapacity);
        vec->code = GROW_VEC(uint8_t, vec->code, oldCapacity, vec->capacity);
        vec->lines = GROW_VEC(int, vec->lines, oldCapacity, vec->capacity);
        vec->columns = GROW_VEC(int, vec->columns, oldCapacity, vec->capacity);
        vec->lengths = GROW_VEC(int, vec->lengths, oldCapacity, vec->capacity);
    }

    vec->code[vec->size] = byte;
    vec->lines[vec->size] = line;
    vec->columns[vec->size] = column;
    vec->lengths[vec->size] = length;
    vec->size++;
}

int pushConstant(CodeVec *vec, Value value) {
    pushValue(&vec->constants, value);
    return vec->constants.size - 1;}