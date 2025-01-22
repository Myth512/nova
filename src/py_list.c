#include "py_list.h"
#include "object_list.h"
#include "value_int.h"
#include "object_string.h"
#include "object_exception.h"
#include "vm.h"

Value PyList_Append(int argc, int kwargc) {
    static char *keywords[] = {"self", "object"};
    Value self, object;
    PARSE_ARGS(&self, &object);

    return List_Append(self, object);
}

Value PyList_Clear(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    ObjList *list = AS_LIST(self);
    freeValueVec(&list->vec);
    initValueVec(&list->vec);

    return NONE_VAL;
}

Value PyList_Copy(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    ObjList *list = AS_LIST(self);
    ObjList *result = allocateList(list->vec.size);

    for (int i = 0; i < list->vec.size; i++)
        result->vec.values[i] = list->vec.values[i];
    
    return OBJ_VAL(result);
}

Value PyList_Count(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    long long count = 0;
    ObjList *list = AS_LIST(self);

    for (int i = 0; i < list->vec.size; i++)
        count += valueToBool(valueEqual(list->vec.values[i], value));
    
    return INT_VAL(count);
}

Value PyList_Extend(int argc, int kwargc) {
    static char *keywords[] = {"self", "iterable"};
    Value self, iterable;
    PARSE_ARGS(&self, &iterable);

    ObjList *list = AS_LIST(self);
    Value iterator = valueIter(iterable);
    Value item = valueNext(iterator);

    while (!isInstance(item, TYPE_CLASS(stopIteration))) {
        List_Append(self, item);
        item = valueNext(iterator);
    }
    
    return NONE_VAL;
}

Value PyList_Index(int argc, int kwargc) {
    static char *keywords[] = {"self", "value", "start", "stop"};
    Value self, value, start, stop;
    PARSE_ARGS(&self, &value, &start, &stop);

    long long istart;
    long long istop;
    ObjList *list = AS_LIST(self);

    if (IS_UNDEFINED(start))
        istart = 0;
    else
        istart = valueToInt(start);
    
    if (IS_UNDEFINED(stop))
        istop = list->vec.size;
    else
        istop = valueToInt(stop);

    for (int i = istart; i < istop; i++) {
        if (valueToBool(valueEqual(list->vec.values[i], value)))
            return INT_VAL(i);
    }

    return createException(VAL_VALUE_ERROR, "%s is not in list", valueToStr(value)->chars);
}

Value PyList_Insert(int argc, int kwargc) {
    static char *keywords[] = {"self", "index", "object"};
    Value self, index, object;
    PARSE_ARGS(&self, &index, &object);

    ObjList *list = AS_LIST(self);
    long long i = calculateIndex(valueToInt(index), list->vec.size);

    if (i == -1)
        i = 0;
    
    if (i == -2)
        i = list->vec.size;

    insertValue(&list->vec, i, object);

    return NONE_VAL;
}

Value PyList_Pop(int argc, int kwargc) {
    static char *keywords[] = {"self", "index"};
    Value self, index;
    PARSE_ARGS(&self, &index);

    ObjList *list = AS_LIST(self);
    long long i;
    
    if (IS_UNDEFINED(index))
        i = list->vec.size - 1;
    else 
        i = calculateIndex(AS_INT(index), list->vec.size);

    Value result = list->vec.values[i];

    List_DelItem(self, INT_VAL(i));

    return result; 
}

Value PyList_Remove(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    ObjList *list = AS_LIST(self);

    for (int i = 0; i < list->vec.size; i++) {
        if (valueToBool(valueEqual(list->vec.values[i], value))) {
            List_DelItem(self, INT_VAL(i));
            return NONE_VAL;
        }
    }

    return createException(VAL_VALUE_ERROR, "list.remove(x): x not in list");
}

Value PyList_Reverse(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    reverseValueVec(&AS_LIST(self)->vec);

    return NONE_VAL;
}

Value PyList_Sort(int argc, int kwargc) {
    return createException(VAL_NOT_IMPLEMENTED_ERROR, "list.sort is not implemented");
}