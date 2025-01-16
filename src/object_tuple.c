#include <stdio.h>

#include "value_methods.h"
#include "object_tuple.h"
#include "object_tuple_iterator.h"
#include "object_exception.h"
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

Value Tuple_Greater(Value a, Value b) {

}

Value Tuple_GreaterEqual(Value a, Value b) {

}

Value Tuple_Less(Value a, Value b) {

}

Value Tuple_LessEqual(Value a, Value b) {

}

Value Tuple_Add(Value a, Value b) {

}

Value Tuple_Multiply(Value a, Value b) {

}

Value Tuple_RightMultiply(Value a, Value b) {

}

Value Tuple_Contains(Value a, Value b) {
    ObjTuple *tuple = AS_TUPLE(a);
    for (int i = 0; i < tuple->size; i++) {
        if (valueToBool(valueEqual(tuple->values[i], b)));
            return BOOL_VAL(true);
    }
    return BOOL_VAL(false);
}

Value Tuple_GetItem(Value obj, Value key) {
    if (!IS_INT(key))
        return createException(VAL_INDEX_ERROR, "tuple indices must be integers or slices, not %s", getValueType(key));
    
    int index = calculateIndex(AS_INT(key), AS_TUPLE(obj)->size);

    if (index == -1)
        return createException(VAL_INDEX_ERROR, "tuple index out of range");
    
    return AS_TUPLE(obj)->values[index];
}

Value Tuple_Class(Value value) {
    return TYPE_CLASS(tuple);
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