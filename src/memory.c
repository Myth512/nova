#include <stdio.h>

#include "memory.h"
#include "common.h"
#include "compiler.h"
#include "vm.h"

#ifdef DEBUG_LOG_GC
    #include "debug.h"
#endif

void* reallocate(void *pointer, size_t oldSize, size_t newSize) {
    if (newSize > oldSize) {
        #ifdef DEBUG_STRESS_GC
            collectGarbage();
        #endif
    }

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
    #ifdef DEBUG_LOG_GC
        printf("%p free type %d\n", object, object->type);
    #endif
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

void markObject(Obj *obj) {
    if (obj == NULL)
        return;

    #ifdef DEBUG_LOG_GC
        pritnf("%p mark ", obj);
        printValue(OBJ_VAL(obj));
        printf("\n");
    #endif

    obj->isMarked = true;
}

void markValue(Value value) {
    if (IS_OBJ(value))
        markObject(AS_OBJ(value));
}

static void markRoots() {
    for (Value *slot = vm.stack; slot < vm.top; slot++) {
        markValue(*slot);
    }

    for (int i = 0; i < vm.frameSize; i++) {
        markObject((Obj*)vm.frames[i].closure);
    }

    for (ObjUpvalue *upvalue = vm.openUpvalues; upvalue != NULL; upvalue = upvalue->next) {
        markObject((Obj*)upvalue);
    }

    markTable(&vm.globals);

    markCompilerRoots();
}

void collectGarbage() {
    #ifdef DEBUG_LOG_GC
        printf("gc begin\n");
    #endif

    markRoots();

    #ifdef DEBUG_LOG_GC
        printf("gc end\n");
    #endif
}