#include <string.h>

#ifdef DEBUG_LOG_GC
    #include <stdio.h>
#endif

#include "object.h"
#include "vm.h"
#include "memory.h"
#include "value.h"
#include "table.h"
#include "common.h"

Obj* allocateObject(size_t size, ValueType type) {
    Obj *object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;
    object->next = vm.objects;
    object->isMarked = false;
    vm.objects = object;
    #ifdef DEBUG_LOG_GC
        printf("allocate %p, size %zu bytes, %s\n", object, size, decodeObjType(OBJ_VAL(object)));
    #endif
    return object;
}

bool isObject(Value value) {
    return value.type < VAL_STRING;
}