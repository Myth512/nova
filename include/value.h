#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef enum {
    // Values
    VAL_UNDEFINED,
    VAL_NONE,
    VAL_BOOL,
    VAL_INT,
    VAL_FLOAT,
    VAL_NOT_IMPLEMENTED,
    // Objects
    VAL_STRING,
    VAL_LIST,
    VAL_TUPLE,
    VAL_FUNCTION,
    VAL_CLOSURE,
    VAL_UPVALUE,
    VAL_NATIVE,
    VAL_CLASS,
    VAL_METHOD,
    VAL_NATIVE_METHOD,
    VAL_INSTANCE,
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

typedef Value (*NativeFn)(int argc, Value *argv);

typedef struct GperfMethod {
	const char *name;
    NativeFn method;
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
    BinaryMethod sub;
    BinaryMethod mul;
    BinaryMethod truediv;
    BinaryMethod floordiv;
    BinaryMethod mod;
    BinaryMethod pow;
    UnaryMethod pos;
    UnaryMethod neg;
    BinaryMethod and;
    BinaryMethod xor;
    BinaryMethod or;
    UnaryMethod invert;
    BinaryMethod lshift;
    BinaryMethod rshift;
    Value (*getattr)(Value, ObjString *name);
    Value (*setattr)(Value, ObjString *name, Value);
    uint64_t (*hash)(Value);
    long long (*len)(Value);
    bool (*toBool)(Value);
    long long (*toInt)(Value);
    double (*toFloat)(Value);
    int (*str)(Value, char*, size_t);
    int (*repr)(Value, char*, size_t);
} ValueMethods;

#define NONE_VAL             ((Value){VAL_NONE})
#define UNDEFINED_VAL       ((Value){VAL_UNDEFINED})
#define NOT_IMPLEMENTED_VAL ((Value){VAL_NOT_IMPLEMENTED})

#define IS_NONE(value)      ((value).type == VAL_NONE)
#define IS_UNDEFINED(value) ((value).type == VAL_UNDEFINED)
#define IS_NOT_IMPLEMENTED(value)  ((value).type == VAL_NOT_IMPLEMENTED)

#define UNARY_WRAPPER(func) \
Value Py##func(int argc, Value *argv) { \
    if (argc != 0) \
        reportArityError(0, 0, argc); \
    Value res = func(argv[0]); \
    if (IS_NOT_IMPLEMENTED(res))        \
        operatorNotImplementedUnary("+", argv[0]); \
    return res; \
}

#define BINARY_WRAPPER(func) \ 
Value Py##func(int argc, Value *argv) { \
    if (argc != 1)                      \
        reportArityError(1, 1, argc);   \
    Value res = func(argv[0], argv[1]); \
    if (IS_NOT_IMPLEMENTED(res))        \
        operatorNotImplemented("+", argv[0], argv[1]); \
    return res; \
}

int writeToBuffer(char *buffer, const size_t size, const char *format, ...);

Value getGperfMethod(Value value, ObjString *name, const struct GperfMethod *(*in_word_set)(register const char*, register size_t));

const char* decodeValueType(Value value);

const char *decodeValueTypeClean(Value value);

#endif