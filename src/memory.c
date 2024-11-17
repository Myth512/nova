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
        printf("%p free type %d ", object, object->type);
        printObject(OBJ_VAL(object));
        printf("\n");
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

static void markVec(ValueVec *vec) {
    for (int i = 0; i < vec->size; i++) {
        markValue(vec->values[i]);
    }
}

static void markReferences(Obj *obj) {
    switch (obj->type) {
        case OBJ_UPVALUE:
            markValue(((ObjUpvalue*)obj)->closed);
            break;
        case OBJ_FUNCTION: {
            ObjFunction *function = (ObjFunction*)obj;
            markObject((Obj*)function->name);
            markVec(&function->code.constants);
            break;
        }
        case OBJ_CLOSURE: {
            ObjClosure *closure = (ObjClosure*)obj;
            markObject((Obj*)closure->function);
            for (int i = 0; i < closure->upvalueCount; i++) {
                markObject((Obj*)closure->upvalues[i]);
            }
            break;
        }
    }
}

void markObject(Obj *obj) {
    if (obj == NULL)
        return;
    if (obj->isMarked)
        return;

    #ifdef DEBUG_LOG_GC
        printf("%p mark ", obj);
        printValue(OBJ_VAL(obj));
        printf("\n");
    #endif

    obj->isMarked = true;
    markReferences(obj);
}

void markValue(Value value) {
    if (IS_OBJ(value))
        markObject(AS_OBJ(value));
}

static void mark() {
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

static void sweep() {
    Obj *prev = NULL, *cur = vm.objects;

    while (cur != NULL) {
        if (cur->isMarked) {
            cur->isMarked = false;
            prev = cur;
            cur = cur->next;
        } else {
            Obj *unreached = cur;
            cur = cur->next;
            if (prev != NULL) {
                prev->next = cur;
            } else {
                vm.objects = cur;
            }
            freeObject(unreached);
        }
    }
}

void collectGarbage() {
    #ifdef DEBUG_LOG_GC
        printf("gc begin\n");
    #endif

    mark();

    sweep();

    #ifdef DEBUG_LOG_GC
        printf("gc end\n");
    #endif
}