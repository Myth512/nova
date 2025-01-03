#ifndef OBJECT_ARRAY_H
#define OBJECT_ARRAY_H

#include "object.h"
#include "value_vector.h"

#define IS_LIST(value)         (value.type == VAL_LIST)

#define AS_LIST(value)         ((ObjList*)value.as.object)

typedef struct {
    Obj obj;
    ValueVec vec;
} ObjList;

#define LIST_METHODS (ValueMethods) { \
    .eq = List_Equal,                 \
    .ne = List_NotEqual,              \
    .gt = List_Greater,               \
    .ge = List_GreaterEqual,          \
    .lt = List_Less,                  \
    .le = List_LessEqual,             \
    .add = List_Add,                  \
    .mul = List_Multiply,             \
    .getitem = List_GetItem,          \
    .setitem = List_SetItem,          \
    .str = List_ToStr,                \
    .repr = List_ToStr                \
}

ObjList* allocateList(int size);

Value List_Equal(Value a, Value b);

Value List_NotEqual(Value a, Value b);

Value List_Greater(Value a, Value b);

Value List_GreaterEqual(Value a, Value b);

Value List_Less(Value a, Value b);

Value List_LessEqual(Value a, Value b);

Value List_Add(Value a, Value b);

Value List_Multiply(Value a, Value b);

Value List_GetItem(Value obj, Value key);

Value List_SetItem(Value obj, Value key, Value value);

int List_ToStr(Value value, char *buffer, size_t size);

#endif