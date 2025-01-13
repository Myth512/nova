#include "object_dict.h"
#include "object_dict_iterator.h"
#include "object_string.h"
#include "object_exception.h"
#include "value_methods.h"
#include "vm.h"
#include "value_int.h"

ObjDict *allocateDict() {
    ObjDict *dict = (ObjDict*)allocateObject(sizeof(ObjDict), VAL_DICT);
    QuadraticTableInit(&dict->table);
    return dict;
}

Value Dict_Contains(Value a, Value b) {
    Value res = QuadraticTableGet(&AS_DICT(a)->table, b);
    return BOOL_VAL(!IS_UNDEFINED(res));
}

Value Dict_Class(Value value) {
    return TYPE_CLASS(dict);
}

Value Dict_Iter(Value value) {
    return OBJ_VAL(allocateDictIterator(value));
}

Value Dict_GetAttr(Value obj, ObjString *name) {

}

Value Dict_GetItem(Value obj, Value key) {
    Value res = QuadraticTableGet(&AS_DICT(obj)->table, key);
    if (IS_UNDEFINED(res))
        return createException(VAL_KEY_ERROR, "'%s'", valueToStr(key)->chars);
    return res;
}

Value Dict_SetItem(Value obj, Value key, Value value) {
    QuadraticTableSet(&AS_DICT(obj)->table, key, value);
    return NONE_VAL;
}

Value Dict_DelItem(Value obj, Value key) {
    bool res = QuadraticTableDelete(&AS_DICT(obj)->table, key);
    if (!res)
        return createException(VAL_KEY_ERROR, "'%s'", valueToStr(key)->chars);
    return NONE_VAL;
}

long long Dict_Len(Value value) {
    return AS_DICT(value)->table.size;
}

bool Dict_ToBool(Value value) {
    return Dict_Len(value);
}

int Dict_ToStr(Value value, char *buffer, size_t size) {
    size_t bytesLeft = size;

    int bytesWritten = writeToBuffer(buffer, bytesLeft, "{");
    bytesLeft -= movePointer(&buffer, bytesWritten);

    size_t capacity = AS_DICT(value)->table.capacity;
    size_t len = AS_DICT(value)->table.size;

    for (int i = 0; i < len; i++) {
        Entry *entry = AS_DICT(value)->table.order[i];

        valueReprWrite(entry->key, buffer, bytesLeft);
        writeToBuffer(buffer, bytesLeft, ": ");
        valueReprWrite(entry->value, buffer, bytesLeft);
        if (i + 1!=  len)
            writeToBuffer(buffer, bytesLeft, ", ");
    }

    writeToBuffer(buffer, bytesLeft, "}");
}