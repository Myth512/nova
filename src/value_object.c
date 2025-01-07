#include "value_object.h"
#include "value_methods.h"
#include "value_int.h"

Value Object_Equal(Value a, Value b) {
    return valueIs(a, b);
}

Value Object_NotEqual(Value a, Value b) {
    return BOOL_VAL(!AS_BOOL(valueIs(a, b)));
}

int Object_ToStr(Value value, char *buffer, const size_t size) {
    writeToBuffer(buffer, size, "%s object at %p", getValueType(value), valueId(value));
}