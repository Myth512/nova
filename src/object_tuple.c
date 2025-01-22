#include <stdio.h>

#include "value_methods.h"
#include "object_tuple.h"
#include "methods_tuple.h"
#include "object_tuple_iterator.h"
#include "object_exception.h"
#include "object_slice.h"
#include "value_int.h"
#include "vm.h"

ObjTuple* allocateTuple(size_t size) {
    ObjTuple *tuple = (ObjTuple*)allocateObject(sizeof(ObjTuple) + size * sizeof(Value), VAL_TUPLE);
    tuple->size = size;
    return tuple;
}

Value Tuple_Equal(Value a, Value b) {
    if (AS_TUPLE(a)->size != AS_TUPLE(b)->size)
        return BOOL_VAL(false);
    for (int i = 0; i < AS_TUPLE(a)->size; i++) {
        if (!valueToBool(valueEqual(AS_TUPLE(a)->values[i], AS_TUPLE(b)->values[i])))
            return BOOL_VAL(false);
    }
    return BOOL_VAL(true);
}

Value Tuple_NotEqual(Value a, Value b) {
    return BOOL_VAL(!AS_BOOL(Tuple_Equal(a, b)));
}

static int min(int a, int b) {
    return a < b ? a : b;
}

static int inequality(ObjTuple *a, ObjTuple *b) {
    int minLength = min(a->size, b->size);
    for (int i = 0; i < minLength; i++) {
        if (valueToBool(valueEqual(a->values[i], b->values[i])))
            continue;
        if (valueToBool(valueGreater(a->values[i], b->values[i])))
            return 1;
        return -1;
    }
    return a->size - b->size;
}

Value Tuple_Greater(Value a, Value b) {
    if (!IS_TUPLE(b))
        return NOT_IMPLEMENTED_VAL;
    return BOOL_VAL(inequality(AS_TUPLE(a), AS_TUPLE(b)) > 0);
}

Value Tuple_GreaterEqual(Value a, Value b) {
    if (!IS_TUPLE(b))
        return NOT_IMPLEMENTED_VAL;
    return BOOL_VAL(inequality(AS_TUPLE(a), AS_TUPLE(b)) >= 0);
}

Value Tuple_Less(Value a, Value b) {
    if (!IS_TUPLE(b))
        return NOT_IMPLEMENTED_VAL;
    return BOOL_VAL(inequality(AS_TUPLE(a), AS_TUPLE(b)) < 0);
}

Value Tuple_LessEqual(Value a, Value b) {
    if (!IS_TUPLE(b))
        return NOT_IMPLEMENTED_VAL;
    return BOOL_VAL(inequality(AS_TUPLE(a), AS_TUPLE(b)) <= 0);
}

Value Tuple_Add(Value a, Value b) {
    if (!IS_TUPLE(b))
        return NOT_IMPLEMENTED_VAL;

    ObjTuple *t1 = AS_TUPLE(a);
    ObjTuple *t2 = AS_TUPLE(b);

    ObjTuple *result = allocateTuple(t1->size + t2->size);

    for (int i = 0; i < t1->size; i++)
        result->values[i] = t1->values[i];
    
    for (int i = 0; i < t2->size; i++)
        result->values[t1->size + i] = t2->values[i];

    return OBJ_VAL(result);
}

Value Tuple_Multiply(Value a, Value b) {
    if (!IS_INT(b))
        return NOT_IMPLEMENTED_VAL;
    
    ObjTuple *tuple = AS_TUPLE(a);
    long long scalar = AS_INT(b);

    ObjTuple *result = allocateTuple(tuple->size * scalar);

    for (int i = 0; i < tuple->size * scalar; i++)
        result->values[i] = tuple->values[i % tuple->size];
    
    return OBJ_VAL(result);
}

Value Tuple_RightMultiply(Value a, Value b) {
    return Tuple_Multiply(a, b);
}

Value Tuple_Contains(Value a, Value b) {
    ObjTuple *tuple = AS_TUPLE(a);
    for (int i = 0; i < tuple->size; i++) {
        if (valueToBool(valueEqual(tuple->values[i], b)))
            return BOOL_VAL(true);
    }
    return BOOL_VAL(false);
}

Value Tuple_GetItem(Value obj, Value key) {
    if (IS_INT(key)) {
        int index = calculateIndex(AS_INT(key), AS_TUPLE(obj)->size);

        if (index < 0)
            return createException(VAL_INDEX_ERROR, "tuple index out of range");
        
        return AS_TUPLE(obj)->values[index];
    } else if (IS_SLICE(key)) {
        ParsedSlice slice = parseSlice(AS_SLICE(key), AS_TUPLE(obj)->size);
        if (slice.step == 0)
            return createException(VAL_VALUE_ERROR, "slice step cannot be zero");
        
        ObjTuple *result = allocateTuple(slice.length);

        if (slice.step > 0) {
            for (int i = slice.start, j = 0; i < slice.stop; i += slice.step, j++) {
                result->values[j] = AS_TUPLE(obj)->values[i];
            }
        } else {
            for (int i = slice.stop - 1, j = 0; i >= slice.start; i += slice.step, j++) {
                result->values[j] = AS_TUPLE(obj)->values[i];
            }
        }
        return OBJ_VAL(result);
    }
    
    return createException(VAL_INDEX_ERROR, "tuple indices must be integers or slices, not %s", getValueType(key));
}

Value Tuple_Class(Value value) {
    return TYPE_CLASS(tuple);
}

Value Tuple_GetAttribute(Value value, ObjString *name) {
    return getStaticAttribute(value, name, in_tuple_set);
}

Value Tuple_Init(Value callee, int argc, Value *argv) {
    if (argc == 0)
        return OBJ_VAL(allocateTuple(0));
    
    long long length = valueLen(argv[0]);

    ObjTuple *tuple = allocateTuple(length);

    Value iterator = valueIter(argv[0]);
    Value item = valueNext(iterator);

    for (int i = 0; i < length; i++) {
        tuple->values[i] = item;
        item = valueNext(iterator);
    }

    return OBJ_VAL(tuple);
}

Value Tuple_Iter(Value value) {
    return OBJ_VAL(allocateTupleIterator(value));
}

int Tuple_Index(Value obj, Value value, int start, int end) {
    size_t size = AS_TUPLE(obj)->size;
    if (end == 0)
        end = size;

    for (int i = start; i < end; i++) {
        if (valueToBool(valueEqual(AS_TUPLE(obj)->values[i], value)))
            return i;
    }
    return -1;
}

long long Tuple_Len(Value value) {
    return AS_TUPLE(value)->size;
}

bool Tuple_ToBool(Value value) {
    return Tuple_Len(value);
}

int Tuple_ToStr(Value value, char *buffer, size_t size) {
    size_t bytesLeft = size;
    int bytesWritten = writeToBuffer(buffer, bytesLeft, "(");
    bytesLeft -= movePointer(&buffer, bytesWritten);
    size_t length = AS_TUPLE(value)->size; 

    if (length == 1) {
        bytesWritten = valueReprWrite(AS_TUPLE(value)->values[0], buffer, bytesLeft);
        bytesLeft -= movePointer(&buffer, bytesWritten);
        writeToBuffer(buffer, bytesLeft, ",)");
        return 0;
    }

    for (int i = 0; i < length; i++) {
        bytesWritten = valueReprWrite(AS_TUPLE(value)->values[i], buffer, bytesLeft);
        bytesLeft -= movePointer(&buffer, bytesWritten);
        if (i + 1 != length) {
            bytesWritten = writeToBuffer(buffer, bytesLeft, ", ");
            bytesLeft -= movePointer(&buffer, bytesWritten);
        }
    }
    writeToBuffer(buffer, size, ")");
}