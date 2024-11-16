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
    OP_GET_UPVALUE,
    OP_SET_UPVALUE,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_GREATER,
    OP_GREATER_EQUAL,
    OP_LESS,
    OP_LESS_EQUAL,
    OP_INCREMENT,
    OP_DECREMENT,
    OP_ADD,
    OP_SUBTRUCT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MOD,
    OP_POWER,
    OP_FACTORIAL,
    OP_ABS,
    OP_NOT,
    OP_NEGATE,
    OP_BUILD_FSTRING,
    OP_TO_STRING,
    OP_JUMP,
    OP_JUMP_TRUE,
    OP_JUMP_TRUE_POP,
    OP_JUMP_FALSE,
    OP_JUMP_FALSE_POP,
    OP_LOOP,
    OP_LOOP_TRUE_POP,
    OP_CALL,
    OP_CLOSURE,
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