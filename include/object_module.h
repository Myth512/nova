#ifndef OBJECT_MODULE_H
#define OBJECT_MODULE_H

#include "table.h"
#include "object.h"
#include "code.h"
#include "object_function.h"

#define IS_MODULE(value) (value.type == VAL_MODULE)

#define AS_MODULE(value) ((ObjModule*)value.as.object)

typedef struct ObjModule {
    Obj obj;
    Table globals;
    ObjFunction *function;
    const char *path;
    const char *source;
};

#define MODULE_METHODS (ValueMethods) { \
    .getattr = Module_GetAttribute,     \
    .call = Module_Call,                \
    .str = Module_ToStr,                \
    .repr = Module_ToStr,               \
}

ObjModule *allocateModule();

Value Module_GetAttribute(Value obj, ObjString *name);

Value Module_Call(Value callee, int argc, int kwargc, Value *argv);

int Module_ToStr(Value value, char *buffer, size_t size);

#endif