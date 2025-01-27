#include "py_dict.h"
#include "object_dict.h"
#include "object_list.h"
#include "object_exception.h"
#include "object_string.h"
#include "vm.h"

Value PyDict_Clear(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    ObjDict *dict = AS_DICT(self);

    tableFree(&dict->table);
    tableInit(&dict->table);

    return NONE_VAL;
}

Value PyDict_Copy(int argc, int kwargc) {
    return createException(VAL_NOT_IMPLEMENTED_ERROR, "dict.copy is not implemented");
}

Value PyDict_FromKeys(int argc, int kwargc) {
    return createException(VAL_NOT_IMPLEMENTED_ERROR, "dict.fromkeys is not implemented");
}

Value PyDict_Get(int argc, int kwargc) {
    static char *keywords[] = {"self", "key", "default"};
    Value self, key, default_;
    PARSE_ARGS(&self, &key, &default_);

    if (IS_UNDEFINED(default_))
        default_ = NONE_VAL;

    Value result = tableGet(&AS_DICT(self)->table, key);

    if (IS_UNDEFINED(result))
        return default_;
    return result;
}

Value PyDict_Items(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    long long length = Dict_Len(self);

    ObjList *list = allocateList(length);

    for (int i = 0; i < length; i++) {
        ObjTuple *tuple = allocateTuple(2);
        tuple->values[0] = AS_DICT(self)->table.order[i]->key;
        tuple->values[1] = AS_DICT(self)->table.order[i]->value;
        list->vec.values[i] = OBJ_VAL(tuple);
    }
    
    return OBJ_VAL(list);
}

Value PyDict_Keys(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    long long length = Dict_Len(self);

    ObjList *list = allocateList(length);

    for (int i = 0; i < length; i++)
        list->vec.values[i] = AS_DICT(self)->table.order[i]->key;
    
    return OBJ_VAL(list);
}

Value PyDict_Pop(int argc, int kwargc) {
    static char *keywords[] = {"self", "key", "default"};
    Value self, key, default_;
    PARSE_ARGS(&self, &key, &default_);

    Value result = tableDelete(&AS_DICT(self)->table, key);

    if (IS_UNDEFINED(result)) {
        if (IS_UNDEFINED(default_))
            return createException(VAL_KEY_ERROR, valueToStr(key)->chars);
        else
            return default_;
    }

    return result;
}

Value PyDict_PopItem(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    ObjDict *dict = AS_DICT(self);
    if (dict->table.size == 0)
        return createException(VAL_KEY_ERROR, "popitem(): dictionary is empty");

    ObjTuple *tuple = allocateTuple(2);
    tuple->values[0] = dict->table.order[dict->table.size - 1]->key;
    tuple->values[1] = dict->table.order[dict->table.size - 1]->value;

    dict->table.order[dict->table.size - 1]->key = (Value){.type=VAL_UNDEFINED, .as.integer=1};
    dict->table.order[dict->table.size - 1]->key = UNDEFINED_VAL;
    dict->table.size--;

    return OBJ_VAL(tuple);
}

Value PyDict_SetDefault(int argc, int kwargc) {
    static char *keywords[] = {"self", "key", "default"};
    Value self, key, default_;
    PARSE_ARGS(&self, &key, &default_);

    if (IS_UNDEFINED(default_))
        default_ = NONE_VAL;

    ObjDict *dict = AS_DICT(self);

    Value result = tableGet(&dict->table, key);

    if (IS_UNDEFINED(result)) {
        tableSet(&dict->table, key, default_);
        return default_;
    }

    return result;
}

Value PyDict_Update(int argc, int kwargc) {
    static char *keywords[] = {"self", "m"};
    Value self, m;
    PARSE_ARGS(&self, &m);

    if (!IS_DICT(m))
        return createException(VAL_TYPE_ERROR, "expect dict");
    
    ObjDict *dest = AS_DICT(self);
    ObjDict *source = AS_DICT(m);

    for (int i = 0; i < source->table.size; i++) {
        Entry *entry = source->table.order[i];
        tableSet(&dest->table, entry->key, entry->value);
    }
    return NONE_VAL;
}

Value PyDict_Values(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    long long length = Dict_Len(self);

    ObjList *list = allocateList(length);

    for (int i = 0; i < length; i++)
        list->vec.values[i] = AS_DICT(self)->table.order[i]->value;
    
    return OBJ_VAL(list);
}