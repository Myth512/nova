#include <stdio.h>

#include "memory.h"
#include "vm.h"

void* reallocate(void *pointer, size_t oldSize, size_t newSize) {
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }

    void *newPointer = realloc(pointer, newSize);
    if (newPointer == NULL) {
        printf("Failed to reallocate %p\n", pointer);
        exit(1);
    }
    return newPointer;
}

static void freeObject(Obj *object) {
    switch (object->type) {
        case OBJ_CLOSURE: {
            ObjClosure *closure = (ObjClosure*)object;
            FREE_VEC(ObjUpvalue*, closure->upvalues, closure->upvalueCount);
            FREE(ObjClosure, closure);
            break;
        }
        case OBJ_FUNCTION: {
            ObjFunction *function = (ObjFunction*)object;
            freeCodeVec(&function->code);
            FREE(ObjFunction, object);
            break;
        }
        case OBJ_NATIVE:
            FREE(ObjNative, object);
            break;
        case OBJ_STRING: {
            ObjString *string = (ObjString*)object;
            FREE(ObjString, object);
            break;
        }
        case OBJ_RAW_STRING: {
            FREE(ObjRawString, object);
            break;
        }
        case OBJ_UPVALUE: {
            FREE(ObjUpvalue, object);
            break;
        }
    }
}

void freeObjects() {
    Obj *object = vm.objects;
    while (object != NULL) {
        Obj *next = object->next;
        freeObject(object);
        object = next;
    }
}