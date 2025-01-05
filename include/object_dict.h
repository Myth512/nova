#ifndef OBJECT_DICT_H
#define OBJECT_DICT_H

#include "value.h"
#include "object.h"
#include "table.h"

#define IS_DICT(value)        ((value).type == VAL_DICT) 

#define AS_DICT(value)        ((ObjDict*)((value).as.object))

#define DICT_METHODS (ValueMethods) { \
    .getattr = Dict_GetAttr,          \
    .getitem = Dict_GetItem,          \
    .setitem = Dict_SetItem,          \
    .str = Dict_ToStr,                \
    .repr = Dict_ToStr,               \
}

typedef struct {
    Obj obj;
    Table table;
} ObjDict;

ObjDict *allocateDict();

Value Dict_GetAttr(Value obj, ObjString *name);

Value Dict_GetItem(Value obj, Value key);

Value Dict_SetItem(Value obj, Value key, Value value);

int Dict_ToStr(Value value, char *buffer, size_t size);

#endif