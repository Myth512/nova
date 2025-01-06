#include <stdio.h>

#include "object_list.h"
#include "object_string.h"
#include "object_class.h"
#include "methods_list.h"
#include "value_int.h"
#include "value_methods.h"
#include "memory.h"
#include "vm.h"

static int min(int a, int b) {
    return a < b ? a : b;
}

ObjList* allocateList(int size) {
    ObjList *list = (ObjList*)allocateObject(sizeof(ObjList), VAL_LIST);
    list->vec.size = size; 
    list->vec.capacity = size; 
    list->vec.values = NULL;
    list->vec.values = (Value*)reallocate(list->vec.values, 0, size * sizeof(Value));
    return list;
}

int List_ToStr(Value value, char *buffer, size_t size) {
    ObjList *list = AS_LIST(value);
    int bytesWritten = writeToBuffer(buffer, size, "[");
    size -= movePointer(&buffer, bytesWritten);
    size_t length = list->vec.size;
    for (int i = 0; i < length; i++) {
        bytesWritten = valueReprWrite(list->vec.values[i], buffer, size);
        size -= movePointer(&buffer, bytesWritten);
        if (i + 1 != length) {
            bytesWritten = writeToBuffer(buffer, size, ", ");
            size -= movePointer(&buffer, bytesWritten);
        }
    }
    writeToBuffer(buffer, size, "]");
}

Value List_Equal(Value a, Value b) {
    if (AS_LIST(a)->vec.size != AS_LIST(b)->vec.size)
        return BOOL_VAL(false);
    for (int i = 0; i < AS_LIST(a)->vec.size; i++) {
        if (!valueToBool(valueEqual(AS_LIST(a)->vec.values[i], AS_LIST(b)->vec.values[i])))
            return BOOL_VAL(false);
    }
    return BOOL_VAL(true);
}

Value List_NotEqual(Value a, Value b) {
    return BOOL_VAL(!AS_BOOL(List_Equal(a, b)));
}

static int inequality(ObjList *a, ObjList *b) {
    int minLength = min(a->vec.size, b->vec.size);
    for (int i = 0; i < minLength; i++) {
        if (valueToBool(valueEqual(a->vec.values[i], b->vec.values[i])))
            continue;
        if (valueToBool(valueGreater(a->vec.values[i], b->vec.values[i])))
            return 1;
        return -1;
    }
    return a->vec.size - b->vec.size;
}

Value List_Greater(Value a, Value b) {
    if (!IS_LIST(b))
        return NOT_IMPLEMENTED_VAL;
    return BOOL_VAL(inequality(AS_LIST(a), AS_LIST(b)) > 0);
}

Value List_GreaterEqual(Value a, Value b) {
    if (!IS_LIST(b))
        return NOT_IMPLEMENTED_VAL;
    return BOOL_VAL(inequality(AS_LIST(a), AS_LIST(b)) >= 0);
}

Value List_Less(Value a, Value b) {
    if (!IS_LIST(b))
        return NOT_IMPLEMENTED_VAL;
    return BOOL_VAL(inequality(AS_LIST(a), AS_LIST(b)) < 0);
}

Value List_LessEqual(Value a, Value b) {
    if (!IS_LIST(b))
        return NOT_IMPLEMENTED_VAL;
    return BOOL_VAL(inequality(AS_LIST(a), AS_LIST(b)) <= 0);
}

Value List_Add(Value a, Value b) {
    if (!IS_LIST(b))
        return NOT_IMPLEMENTED_VAL;

    ObjList *l1 = AS_LIST(a);
    ObjList *l2 = AS_LIST(b);

    size_t size = l1->vec.size + l2->vec.size; 

    ObjList *result = allocateList(size);

    for (int i = 0; i < l1->vec.size; i++)
        result->vec.values[i] = l1->vec.values[i];

    for (int i = 0; i < l1->vec.size; i++)
        result->vec.values[l1->vec.size + i] = l2->vec.values[i];

    return OBJ_VAL(result);
}

Value List_Multiply(Value a, Value b) {
    if (!IS_INT(b))
        return NOT_IMPLEMENTED_VAL;

    size_t oldSize = AS_LIST(a)->vec.size;
    size_t newSize = oldSize * AS_INT(b);

    ObjList *result = allocateList(newSize);

    for (int i = 0; i < newSize; i++)
        result->vec.values[i] = AS_LIST(a)->vec.values[i % oldSize];

    return OBJ_VAL(result);
}

Value List_RightMultiply(Value a, Value b) {
    return List_Multiply(a, b);
}

Value List_Contains(Value a, Value b) {
    ObjList *list = AS_LIST(a);
    for (int i = 0; i < list->vec.size; i++) {
        if (valueToBool(valueEqual(list->vec.values[i], b)))
            return BOOL_VAL(true);
    }
    return BOOL_VAL(false);
}

Value List_Class(Value value) {
    return OBJ_VAL(vm.types.list);
}

Value List_GetAttr(Value list, ObjString *name) {
    return getGperfMethod(list, name, in_list_set);
}

Value List_GetItem(Value obj, Value key) {
    if (!IS_INT(key))
        reportRuntimeError("Index must be integer");
    
    int index = calculateIndex(AS_INT(key), AS_LIST(obj)->vec.size);

    if (index == -1)
        reportRuntimeError("Index is out of range");
    
    return AS_LIST(obj)->vec.values[index];
}

Value List_SetItem(Value obj, Value key, Value value) {
    if (!IS_INT(key))
        reportRuntimeError("Index must be integer");
    
    int index = calculateIndex(AS_INT(key), AS_LIST(obj)->vec.size);

    if (index == -1)
        reportRuntimeError("Index is out of range");
    
    AS_LIST(obj)->vec.values[index] = value;
    return NONE_VAL;
}

Value List_Append(Value obj, Value value) {
    pushValue(&AS_LIST(obj)->vec, value);
    return NONE_VAL;
}

Value List_Pop(Value obj) {
    return popValue(&AS_LIST(obj)->vec);
}

Value List_Sort(Value obj) {

}

long long List_Len(Value value) {
    return AS_LIST(value)->vec.size;
}

bool List_ToBool(Value value) {
    return List_Len(value);
}