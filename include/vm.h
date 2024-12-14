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
    ObjString *init;
    ObjString *add;
    ObjString *ladd;
    ObjString *radd;
    ObjString *inc;
    ObjString *dec;
    ObjString *sub;
    ObjString *lsub;
    ObjString *rsub;
    ObjString *neg;
    ObjString *mul;
    ObjString *lmul;
    ObjString *rmul;
    ObjString *div;
    ObjString *ldiv;
    ObjString *rdiv;
    ObjString *mod;
    ObjString *lmod;
    ObjString *rmod;
    ObjString *pow;
    ObjString *lpow;
    ObjString *rpow;
    ObjString *eq;
    ObjString *ne;
    ObjString *lt;
    ObjString *le;
    ObjString *gt;
    ObjString *ge;
    ObjString *call;
    ObjString *getat;
    ObjString *setat;
    ObjString *len;
    ObjString *str;
} MagicStrings;

typedef struct {
    const char *source;
    CallFrame frames[FRAMES_SIZE];
    int frameSize;
    CodeVec *code;
    uint8_t *ip;
    Value stack[STACK_SIZE];
    Value *top;
    Table globals;
    Table strings;
    MagicStrings magicStrings;
    ObjUpvalue *openUpvalues;
    Obj *objects;
    size_t bytesAllocated;
    size_t nextGC;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

extern VM vm;

void push(Value value);

Value pop();

void initVM();

void freeVM();

void printInstanse(Value value);

InterpretResult interpret(const char *source);

#endif