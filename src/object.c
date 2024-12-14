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


Obj* allocateObject(size_t size, ObjType type) {
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

static char convertToEscapeChar(char c) {
    switch (c) {
        case 'a':
            return '\a';
        case 'b':
            return '\n';
        case 'f':
            return '\f';
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        case 'v':
            return '\v';
        case '0':
            return '\0';
        case 'e':
            return '\e';
        default:
            return c;
    }
}

int resolveEscapeSequence(const char *source, int sourceLength, char *destination) {
    const char *sourceChar = source;
    const char *endChar = source + sourceLength;
    char *destinationChar = destination;
    int destinationLength = 0;
    while (sourceChar < endChar) {
        if (*sourceChar != '\\') {
            *destinationChar = *sourceChar;
            sourceChar++;
            destinationChar++;
            destinationLength++;
        } else {
            sourceChar++;
            *destinationChar = convertToEscapeChar(*sourceChar);
            sourceChar++;
            destinationChar++;
            destinationLength++;
        }
    }
    *destinationChar = '\0';
    return destinationLength;
}

static uint64_t hashString(const char *value) {
    uint64_t hash = 0;
    while (*value) {
        hash = (hash * 31) + (unsigned char)(*value); 
        value++;  
    }
    return hash;
}

uint32_t getHash(ObjString *string) {
    if (!string->isHashed) {
        string->hash = hashString(string->chars);
        string->isHashed = true;
    }
    return string->hash;
}

ObjString* allocateString(const char *chars, int length) {
    int size = sizeof(ObjString) + length + 1;
    ObjString *string = (ObjString*)allocateObject(size, OBJ_STRING); 
    string->isInterned = false;
    string->isHashed = false;

    string->length = resolveEscapeSequence(chars, length, string->chars);
    int newSize = size - (length - string->length);
    reallocate(string, size, newSize);

    return string;
}

ObjString* copyString(const char *chars, int length) {
    return allocateString(chars, length);
}

ObjString* takeString(char *chars, int length) {
    // uint32_t hash = hashString(chars, length);
    // ObjString *interned = tableFindString(&vm.strings, chars, length, hash);

    // if (interned != NULL) {
    //     FREE_VEC(char, chars, length + 1);
    //     return interned;
    // }

    return allocateString(chars, length);
}

ObjArray* allocateArray(int size) {
    ObjArray *array = (ObjArray*)allocateObject(sizeof(ObjArray), OBJ_ARRAY);
    array->values.size = size; 
    array->values.capacity = size; 
    array->values.values = (Value*)reallocate(array->values.values, 0, size * sizeof(Value));
    return array;
}

ObjRawString* createRawString(const char *chars, int length) {
    ObjRawString *string = (ObjRawString*)allocateObject(sizeof(ObjRawString), OBJ_RAW_STRING);
    string->chars = chars;
    string->length = length;
    return string;
}

bool compareStrings(ObjString *a, ObjString *b) {
    if (a->isInterned && b->isInterned)
        return a == b;
    if (a->length != b->length) 
        return false;
    return memcmp(a->chars, b->chars, a->length) == 0;
}

bool compareArrays(ObjArray *a, ObjArray *b) {
    if (a->values.size != b->values.size)
        return false;
    for (int i = 0; i < a->values.size; i++) {
        if (!compareValues(a->values.values[i], b->values.values[i]))
            return false;
    }
    return true;
}

ObjUpvalue *createUpvalue(Value *slot) {
    ObjUpvalue *upvalue = (ObjUpvalue*)allocateObject(sizeof(ObjUpvalue), OBJ_UPVALUE);
    upvalue->closed = NIL_VAL;
    upvalue->location = slot;
    upvalue->next = NULL;
    return upvalue;
}

ObjFunction* createFunction() {
    ObjFunction *function = (ObjFunction*)allocateObject(sizeof(ObjFunction), OBJ_FUNCTION);
    function->arity = 0;
    function->upvalueCount = 0;
    function->name = NULL;
    initCodeVec(&function->code);
    return function;
}

ObjClosure* createClosure(ObjFunction *function) {
    ObjUpvalue **upvalues = ALLOCATE(ObjUpvalue*, function->upvalueCount); 
    for (int i = 0; i < function->upvalueCount; i++)
        upvalues[i] = NULL;

    ObjClosure *closure = (ObjClosure*)allocateObject(sizeof(ObjClosure), OBJ_CLOSURE);
    closure->function = function;
    closure->upvalues = upvalues;
    closure->upvalueCount = function->upvalueCount;
    return closure;
}

ObjNative* createNative(NativeFn function, const char *name) {
    ObjNative *native = (ObjNative*)allocateObject(sizeof(ObjNative), OBJ_NATIVE);
    native->function = function;
    native->name = name;
    return native;
}

ObjClass *createClass(ObjString *name) {
    ObjClass *class = (ObjClass*)allocateObject(sizeof(ObjClass), OBJ_CLASS);
    class->name = name;
    initTable(&class->methods);
    return class;
}

void printFunction(ObjFunction *function) {
    if (function->name == NULL) {
        printf("<script>");
        return;
    }
    printf("<function %s>", function->name->chars);
}

ObjInstance *createInstance(ObjClass *class) {
    ObjInstance *instance = (ObjInstance*)allocateObject(sizeof(ObjInstance), OBJ_INSTANCE);
    instance->class = class;
    initTable(&instance->fields);
    return instance;
}

ObjMethod *createMethod(Value reciever, ObjClosure *function) {
    ObjMethod *method = (ObjMethod*)allocateObject(sizeof(ObjMethod), OBJ_METHOD);
    method->reciever = reciever;
    method->method = function;
    return method;
}

ObjNativeMethod *createNativeMethod(Value reciever, NativeFn function, const char *name) {
    ObjNativeMethod *native = (ObjNativeMethod*)allocateObject(sizeof(ObjNativeMethod), OBJ_NATIVE_METHOD);
    native->reciever = reciever;
    native->name = name;
    native->method = function;
    return native;
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_CLOSURE:
            printFunction(AS_CLOSURE(value)->function);
            break;
        case OBJ_FUNCTION:
            printFunction(AS_FUNCTION(value));
            break;
        case OBJ_NATIVE:
            printf("<native function %s>", AS_NATIVE(value)->name);
            break;
        case OBJ_NATIVE_METHOD:
            printf("<native method %s>", AS_NATIVE_METHOD(value)->name);
            break;
        case OBJ_STRING:
            printf("%s", AS_CSTRING(value));
            break;
        case OBJ_RAW_STRING: {
            ObjRawString *string = AS_RAW_STRING(value);
            printf("%.*s\n", string->length, string->chars);
            break;
        }
        case OBJ_UPVALUE:
            printValue(*(((ObjUpvalue*)AS_OBJ(value))->location));
            break;
        case OBJ_ARRAY: {
            printf("[");
            size_t size = AS_ARRAY(value)->values.size;
            for (int i = 0; i < size; i++) {
                printValue(AS_ARRAY(value)->values.values[i]);
                if (i + 1 != size) {
                    printf(", ");
                }
            }
            printf("]");
            break;
        }
        case OBJ_CLASS:
            printf("%s", AS_CLASS(value)->name->chars);
            break;
        case OBJ_INSTANCE:
            printInstanse(value);
            break;
        case OBJ_METHOD:
            printFunction(AS_METHOD(value)->method->function);
            break;
    }
}

int writeObject(Value value, char *buffer, const size_t maxSize) {
    switch (OBJ_TYPE(value)) {
        case OBJ_FUNCTION:
            ObjFunction *function = AS_FUNCTION(value);
            return snprintf(buffer, maxSize, "<function %s>", function->name->chars);
        case OBJ_NATIVE:
            return snprintf(buffer, maxSize, "<native function %s>", AS_NATIVE(value)->name);
        case OBJ_STRING:
            return snprintf(buffer, maxSize, "%s", AS_CSTRING(value));
        case OBJ_RAW_STRING:
            return resolveEscapeSequence(AS_RAW_STRING(value)->chars, AS_RAW_STRING(value)->length, buffer);
    }
    return -1; // unreachable
}

const char* decodeObjType(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_NATIVE:
            return "<type native>";
        case OBJ_FUNCTION:
            return "<type function>";
        case OBJ_STRING:
            return "<type string>";
        case OBJ_RAW_STRING:
            return "<type raw string>";
        case OBJ_UPVALUE:
            return "<type upvalue>";
        case OBJ_CLOSURE:
            return "<type closure>";
        case OBJ_ARRAY:
            return "<type array>";
        case OBJ_CLASS:
            return "<type class>";
        case OBJ_INSTANCE:
            return "<type instance>";
    }
    return "invalid type"; // uncreachable
}

const char *decodeObjTypeClean(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_NATIVE:
            return "native";
        case OBJ_STRING:
            return "string";
        case OBJ_UPVALUE:
            return "upvalue";
        case OBJ_CLOSURE:
            return "function";
        case OBJ_ARRAY:
            return "array";
        case OBJ_CLASS:
            return "class";
        case OBJ_INSTANCE:
            return AS_INSTANCE(value)->class->name->chars;
    }
    return "invalid type"; // uncreachable
}

uint64_t hashObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_NATIVE:
            return hashLong((long)AS_NATIVE(value)->function);
        case OBJ_STRING:
            return getHash(AS_STRING(value)); 
        case OBJ_CLOSURE:
            return hashLong((long)AS_CLOSURE(value)->function);
        default:
            return 0;
    }
}

bool compareObjects(Value a, Value b) {
    switch (a.type) {
        case OBJ_NATIVE:
            return AS_NATIVE(a)->function == AS_NATIVE(b)->function;
        case OBJ_CLOSURE:
            return AS_CLOSURE(a)->function == AS_CLOSURE(b)->function;
        case OBJ_STRING:
            return compareStrings(AS_STRING(a), AS_STRING(b));
        case OBJ_ARRAY:
            return compareArrays(AS_ARRAY(a), AS_ARRAY(b));
    }
}