#ifndef OBJECT_DICT_H
#define OBJECT_DICT_H

#include "value.h"
#include "object.h"
#include "table.h"

#define IS_DICT(value)        ((value).type == VAL_DICT) 

#define AS_DICT(value)        ((ObjDict*)((value).as.object))

#define DICT_METHODS (ValueMethods) { \
    .contains = Dict_Contains,        \
    .class = Dict_Class,              \
    .iter = Dict_Iter,                \
    .getattr = Dict_GetAttr,          \
    .getitem = Dict_GetItem,          \
    .setitem = Dict_SetItem,          \
    .delitem = Dict_DelItem,          \
    .len = Dict_Len,                  \
    .toBool = Dict_ToBool,            \
    .str = Dict_ToStr,                \
    .repr = Dict_ToStr,               \
}

typedef struct {
    Obj obj;
    Table table;
} ObjDict;

ObjDict *allocateDict();

Value Dict_Contains(Value a, Value b);

Value Dict_Class(Value value);

Value Dict_Iter(Value value);

Value Dict_GetAttr(Value obj, ObjString *name);

Value Dict_GetItem(Value obj, Value key);

Value Dict_SetItem(Value obj, Value key, Value value);

Value Dict_DelItem(Value obj, Value key);

long long Dict_Len(Value value);

bool Dict_ToBool(Value value);

int Dict_ToStr(Value value, char *buffer, size_t size);

#endif