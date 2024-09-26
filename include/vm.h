#ifndef VM_H
#define VM_H

#include "common.h"
#include "code.h"
#include "table.h"

#define STACK_SIZE 256

typedef struct {
    CodeVec *code;
    uint8_t *ip;
    Value stack[STACK_SIZE];
    Value *top;
    Table globals;
    Table strings;
    Obj *objects;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

extern VM vm;

void initVM();

void freeVM();

InterpretResult interpret(const char *source);

#endif