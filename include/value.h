#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef enum {
    // Values
    VAL_OBJECT,
    VAL_UNDEFINED,
    VAL_NONE,
    VAL_BOOL,
    VAL_INT,
    VAL_FLOAT,
    VAL_NOT_IMPLEMENTED,
    VAL_TYPE,
    // Objects
    VAL_STRING,
    VAL_STRING_ITERATOR,
    VAL_LIST,
    VAL_LIST_ITERATOR,
    VAL_TUPLE,
    VAL_TUPLE_ITERATOR,
    VAL_DICT,
    VAL_DICT_ITERATOR,
    VAL_FUNCTION,
    VAL_CLOSURE,
    VAL_UPVALUE,
    VAL_NATIVE,
    VAL_CLASS,
    VAL_NATIVE_CLASS,
    VAL_METHOD,
    VAL_NATIVE_METHOD,
    VAL_INSTANCE,
    VAL_SUPER,
    VAL_RANGE,
    VAL_RANGE_ITERATOR,
    VAL_EXCEPTION,
    VAL_ZERO_DIVISON_ERROR,
    VAL_STOP_ITERATION,
} ValueType;

typedef struct {
    ValueType type;
    union {
        double floating;
        long long integer;
        Obj *object;
    } as;
} Value;

typedef struct {
    bool hasValue;
    Value value;
} OptValue;

typedef Value (*NativeFn)(int argc, int kwargc);

typedef struct GperfMethod {
	const char *name;
    Value (*method)(int, Value*);
} GperfMethod;

typedef Value (*BinaryMethod)(Value, Value);
typedef Value (*UnaryMethod)(Value);

typedef struct {
    BinaryMethod eq;
    BinaryMethod ne;
    BinaryMethod gt;
    BinaryMethod ge;
    BinaryMethod lt;
    BinaryMethod le;
    BinaryMethod add;
    BinaryMethod radd;
    BinaryMethod sub;
    BinaryMethod rsub;
    BinaryMethod mul;
    BinaryMethod rmul;
    BinaryMethod truediv;
    BinaryMethod rtruediv;
    BinaryMethod floordiv;
    BinaryMethod rfloordiv;
    BinaryMethod mod;
    BinaryMethod rmod;
    BinaryMethod pow;
    BinaryMethod rpow;
    UnaryMethod pos;
    UnaryMethod neg;
    BinaryMethod and;
    BinaryMethod rand;
    BinaryMethod xor;
    BinaryMethod rxor;
    BinaryMethod or;
    BinaryMethod ror;
    UnaryMethod invert;
    BinaryMethod lshift;
    BinaryMethod rlshift;
    BinaryMethod rshift;
    BinaryMethod rrshift;
    BinaryMethod contains;
    Value (*init)(Value, int, Value*);
    Value (*call)(Value, int, int, Value*);
    UnaryMethod class;
    UnaryMethod iter;
    UnaryMethod next;
    Value (*getattr)(Value, ObjString *name);
    Value (*setattr)(Value, ObjString *name, Value);
    Value (*delattr)(Value, ObjString *name);
    Value (*getitem)(Value, Value);
    Value (*setitem)(Value, Value, Value);
    Value (*delitem)(Value, Value);
    uint64_t (*hash)(Value);
    long long (*len)(Value);
    bool (*toBool)(Value);
    long long (*toInt)(Value);
    double (*toFloat)(Value);
    int (*str)(Value, char*, size_t);
    int (*repr)(Value, char*, size_t);
} ValueMethods;

#define NONE_VAL             ((Value){.type=VAL_NONE, .as.integer=0})
#define UNDEFINED_VAL       ((Value){.type=VAL_UNDEFINED, .as.integer=0})
#define NOT_IMPLEMENTED_VAL ((Value){.type=VAL_NOT_IMPLEMENTED, .as.integer=0})

#define IS_NONE(value)      ((value).type == VAL_NONE)
#define IS_UNDEFINED(value) ((value).type == VAL_UNDEFINED)
#define IS_NOT_IMPLEMENTED(value)  ((value).type == VAL_NOT_IMPLEMENTED)

#define UNARY_WRAPPER(name)             \
Value Py##name(int argc, Value *argv) { \
    if (argc != 0)                      \
        reportArityError(0, 0, argc);   \
    Value (*method)(Value) = name;      \
    if (method == NULL)                 \
        reportRuntimeError("ns");       \
    return method(argv[0]);             \
}

#define BINARY_WRAPPER(name)              \
Value Py##name(int argc, Value *argv) {   \
    if (argc != 1)                        \
        reportArityError(1, 1, argc);     \
    Value (*method)(Value, Value) = name; \
    if (method == NULL)                   \
        reportRuntimeError("ns");         \
    return method(argv[0], argv[1]);      \
}

int writeToBuffer(char *buffer, const size_t size, const char *format, ...);

Value getGperfMethod(Value value, ObjString *name, const struct GperfMethod *(*in_word_set)(register const char*, register size_t));

int calculateIndex(int index, int length);

int movePointer(char **buffer, int bytesWritten);

#endif