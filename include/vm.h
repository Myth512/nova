#ifndef VM_H
#define VM_H

#include <math.h>

#include "common.h"
#include "debug.h"
#include "code.h"
#include "compiler.h"

#define STACK_SIZE 256

typedef struct {
    CodeVec *code;
    uint8_t *ip;
    Value stack[STACK_SIZE];
    Value *top;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

void initVM();

void freeVM();

InterpretResult interpret(const char *source);

#endif