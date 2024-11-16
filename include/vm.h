#ifndef VM_H
#define VM_H

#include "common.h"
#include "code.h"
#include "table.h"
#include "object.h"

#define FRAMES_SIZE 64
#define STACK_SIZE (FRAMES_SIZE * UINT8_MAX)

typedef struct {
    ObjClosure *closure;
    uint8_t *ip;
    Value *slots;
} CallFrame;

typedef struct {
    CallFrame frames[FRAMES_SIZE];
    int frameSize;
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