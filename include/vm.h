#ifndef VM_H
#define VM_H

#include "common.h"
#include "code.h"
#include "name_table.h"
#include "object.h"
#include "object_function.h"
#include "object_class.h"

#define FRAMES_SIZE 64
#define STACK_SIZE (FRAMES_SIZE * UINT8_MAX)

typedef struct {
    ObjClosure *closure;
    uint8_t *ip;
    Value *slots;
    uint8_t *exceptAddr;
    bool isMethod;
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
    ObjString *bool_;
    ObjString *str;
    ObjString *int_;
    ObjString *float_;
} MagicStrings;

typedef struct {
    ObjNativeClass *object;
    ObjNativeClass *none;
    ObjNativeClass *bool_;
    ObjNativeClass *int_;
    ObjNativeClass *float_;
    ObjNativeClass *type;
    ObjNativeClass *str;
    ObjNativeClass *list;
    ObjNativeClass *tuple;
    ObjNativeClass *dict;
    ObjNativeClass *exception;
    ObjNativeClass *zeroDivisionError;
    ObjNativeClass *stopIteration;
} BaseTypes;

typedef struct {
    const char *source;
    CallFrame frames[FRAMES_SIZE];
    int frameSize;
    CodeVec *code;
    uint8_t *ip;
    Value stack[STACK_SIZE];
    Value *top;
    NameTable globals;
    NameTable strings;
    MagicStrings magicStrings;
    BaseTypes types;
    ObjUpvalue *openUpvalues;
    Obj *objects;
    size_t bytesAllocated;
    size_t nextGC;
    bool allowStackPrinting;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

extern VM vm;
extern CallFrame *frame;

void push(Value value);

Value pop();

void insert(int distance, Value value);

void raise();

void reportRuntimeError(const char *format, ...);

void reportArityError(int min, int max, int got);

void operatorNotImplemented(char *operator, Value a, Value b);

void operatorNotImplementedUnary(char *operator, Value a);

void functionNotImplemented(char *function, Value a);

void call(ObjClosure *closure, int argc, int kwargc, bool isMethod);

Value callNovaValue(Value callee, int argc);

OptValue callNovaMethod(Value obj, ObjString *methodName);

OptValue callNovaMethod1arg(Value obj, ObjString *methodName, Value arg);

OptValue callNovaMethod2args(Value obj, ObjString *methodName, Value arg1, Value arg2);

void initVM();

void freeVM();

InterpretResult interpret(const char *source);

#endif