#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>

#include "value.h"
#include "value_vector.h"

typedef enum {
    OP_CONSTANT,
    OP_NONE,
    OP_POP,
    OP_TRUE,
    OP_FALSE,
    OP_GET_GLOBAL,
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
    OP_ADD,
    OP_SUBTRUCT,
    OP_MULTIPLY,
    OP_POWER,
    OP_TRUE_DIVIDE,
    OP_FLOOR_DIVIDE,
    OP_MOD,
    OP_POSITIVE,
    OP_NEGATIVE,
    OP_BITWISE_AND,
    OP_BITWISE_XOR,
    OP_BITWISE_OR,
    OP_LEFT_SHIFT,
    OP_RIGHT_SHIFT,
    OP_INVERT,
    OP_NOT,
    OP_BUILD_FSTRING,
    OP_BUILD_LIST,
    OP_BUILD_TUPLE,
    OP_GET_ITEM,
    OP_GET_ITEM_NO_POP,
    OP_SET_ITEM,
    OP_JUMP,
    OP_JUMP_TRUE,
    OP_JUMP_TRUE_POP,
    OP_JUMP_FALSE,
    OP_JUMP_FALSE_POP,
    OP_LOOP,
    OP_LOOP_TRUE_POP,
    OP_CALL,
    OP_CLOSURE,
    OP_CLOSE_UPVALUE,
    OP_CLASS,
    OP_METHOD,
    OP_GET_ATTRIBUTE, 
    OP_SET_ATTRIBUTE,
    OP_RETURN,
} OpCode;

typedef struct {
    int size;
    int capacity;
    uint8_t *code;
    int *lines;
    int *columns;
    int *lengths;
    ValueVec constants;
} CodeVec;

void initCodeVec(CodeVec *code);

void freeCodeVec(CodeVec *codeVec);

void pushInstruction(CodeVec *codeVec, uint8_t byte, int row, int column, int len);

int pushConstant(CodeVec *vec, Value value);

#endif