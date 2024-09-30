#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>

#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_NIL,
    OP_POP,
    OP_TRUE,
    OP_FALSE,
    OP_GET_GLOBAL,
    OP_DEFINE_GLOBAL,
    OP_SET_GLOBAL,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_GREATER,
    OP_GREATER_EQUAL,
    OP_LESS,
    OP_LESS_EQUAL,
    OP_ADD,
    OP_SUBTRUCT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MOD,
    OP_NOT,
    OP_NEGATE,
    OP_PRINT,
    OP_BUILD_FSTRING,
    OP_APPEND_TO_STRING,
    OP_BUILD_STOP,
    OP_TO_STRING,
    OP_RETURN,
} OpCode;

typedef struct {
    int size;
    int capacity;
    uint8_t *code;
    int *lines;
    ValueVec constants;
} CodeVec;

void initCodeVec(CodeVec *code);

void freeCodeVec(CodeVec *codeVec);

void pushInstruction(CodeVec *codeVec, uint8_t byte, int line);

int pushConstant(CodeVec *vec, Value value);

#endif