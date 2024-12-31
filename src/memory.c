#include <stdio.h>

#include "memory.h"
#include "common.h"
#include "compiler.h"
#include "object_string.h"
#include "object_class.h"
#include "object_instance.h"
#include "vm.h"

#define GC_HEAP_GROW_FACTOR 2

int indent = 0;

static void printIndent() {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    printf("└─ ");
}

#ifdef DEBUG_LOG_GC
    #include "debug.h"
#endif

void* reallocate(void *pointer, size_t oldSize, size_t newSize) {
    vm.bytesAllocated += newSize - oldSize;
    if (newSize > oldSize) {
        #ifdef DEBUG_STRESS_GC
            collectGarbage();
        #endif
    }
    if (vm.bytesAllocated > vm.nextGC) {
        collectGarbage();
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
        printf("free %p, type %s, data ", object, decodeObjType(OBJ_VAL(object)));
        printObject(OBJ_VAL(object));
        printf("\n");
    #endif
    switch (object->type) {
        case VAL_CLOSURE: {
            ObjClosure *closure = (ObjClosure*)object;
            FREE_VEC(ObjUpvalue*, closure->upvalues, closure->upvalueCount);
            FREE(ObjClosure, closure);
            break;
        }
        case VAL_FUNCTION: {
            ObjFunction *function = (ObjFunction*)object;
            freeCodeVec(&function->code);
            FREE(ObjFunction, object);
            break;
        }
        case VAL_NATIVE:
            FREE(ObjNative, object);
            break;
        case VAL_STRING: {
            ObjString *string = (ObjString*)object;
            FREE(ObjString, object);
            break;
        }
        case VAL_UPVALUE: {
            FREE(ObjUpvalue, object);
            break;
        }
        case VAL_CLASS: {
            FREE(ObjClass, object);
            break;
        }
        case VAL_INSTANCE: {
            ObjInstance *instance = (ObjInstance*)object;
            freeTable(&instance->fields);
            FREE(ObjInstance, object);
            break;
        }
        case VAL_METHOD: {
            FREE(ObjMethod, object);
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

static void markVec(ValueVec *vec) {
    for (int i = 0; i < vec->size; i++) {
        markValue(vec->values[i]);
    }
}

static void markReferences(Obj *obj) {
    switch (obj->type) {
        case VAL_UPVALUE:
            markValue(((ObjUpvalue*)obj)->closed);
            break;
        case VAL_FUNCTION: {
            ObjFunction *function = (ObjFunction*)obj;
            #ifdef DEBUG_LOG_GC
                printIndent();
                printf("name\n");
                indent++;
            #endif
            markObject((Obj*)function->name);
            #ifdef DEBUG_LOG_GC
                indent--;
                printIndent();
                printf("constants\n");
                indent++;
            #endif
            markVec(&function->code.constants);
            #ifdef DEBUG_LOG_GC
                indent--;
            #endif
            break;
        }
        case VAL_CLOSURE: {
            ObjClosure *closure = (ObjClosure*)obj;
            #ifdef DEBUG_LOG_GC
                printIndent();
                printf("function\n");
                indent++;
            #endif
            markObject((Obj*)closure->function);
            #ifdef DEBUG_LOG_GC
                indent--;
                printIndent();
                printf("upvalues\n");
                indent++;
            #endif
            for (int i = 0; i < closure->upvalueCount; i++) {
                markObject((Obj*)closure->upvalues[i]);
            }
            #ifdef DEBUG_LOG_GC
                indent--;
            #endif
            break;
        }
        case VAL_CLASS: {
            ObjClass *class = (ObjClass*)obj;
            markObject((Obj*)class->name);
            break;
        }
        case VAL_INSTANCE: {
            ObjInstance *instance = (ObjInstance*)obj;
            markObject((Obj*)instance->class);
            markTable(&instance->fields);
            break;
        }
        case VAL_METHOD: {
            ObjMethod *method = (ObjMethod*)obj;
            markValue(method->reciever);
            markObject((Obj*)method->method);
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
        printIndent();
        printf("mark %p, %s, data ", obj, decodeObjType(OBJ_VAL(obj)));
        printValue(OBJ_VAL(obj));
        printf("\n");
    #endif

    obj->isMarked = true;

    #ifdef DEBUG_LOG_GC
        indent++;
    #endif
    markReferences(obj);
    #ifdef DEBUG_LOG_GC
        indent--;
    #endif
}

void markValue(Value value) {
    if (isObject(value))
        markObject(value.as.object);
}

static void mark() {
    #ifdef DEBUG_LOG_GC
        printf("\033[31mmark stack\n");
        indent++;
    #endif
    for (Value *slot = vm.stack; slot < vm.top; slot++) {
        markValue(*slot);
    }

    #ifdef DEBUG_LOG_GC
        indent--;
        printf("\033[32mmark frames\n");
        indent++;
    #endif
    for (int i = 0; i < vm.frameSize; i++) {
        markObject((Obj*)vm.frames[i].closure);
    }

    #ifdef DEBUG_LOG_GC
        indent--;
        printf("\033[33mmark upvalues\n");
        indent++;
    #endif
    for (ObjUpvalue *upvalue = vm.openUpvalues; upvalue != NULL; upvalue = upvalue->next) {
        markObject((Obj*)upvalue);
    }

    #ifdef DEBUG_LOG_GC
        indent++;
        printf("\033[34mmark globals\n");
        indent--;
    #endif
    markTable(&vm.globals);

    #ifdef DEBUG_LOG_GC
        indent--;
        printf("\033[35mmark compiler\n");
        indent++;
    #endif
    markCompilerRoots();

    #ifdef DEBUG_LOG_GC
        indent--;
        printf("\033[0m");
    #endif

    markObject((Obj*)vm.magicStrings.init);
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
        size_t before = vm.bytesAllocated;
    #endif

    mark();

    sweep();

    vm.nextGC = vm.bytesAllocated * GC_HEAP_GROW_FACTOR;

    #ifdef DEBUG_LOG_GC
        printf("gc end\n");
        printf("collected %zu bytes (from %zu to %zu) next at %zu", before - vm.bytesAllocated, before, vm.bytesAllocated, vm.nextGC);
    #endif
}