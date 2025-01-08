#include "object_string_iterator.h"
#include "object_string.h"
#include "object_exception.h"
#include "vm.h"

ObjStringIterator *allocateStringIterator(Value value) {
    ObjStringIterator *iter = (ObjStringIterator*)allocateObject(sizeof(ObjStringIterator), VAL_STRING_ITERATOR);
    iter->current = AS_STRING(value)->chars;
}

Value StringIterator_Iter(Value value) {
    return value;
}

Value StringIterator_Next(Value value) {
    ObjStringIterator *iter = AS_STRING_ITERATOR(value);
    char res = *iter->current;
    if (res == '\0')
        return createMsgException("", VAL_STOP_ITERATION);
    iter->current++;
    return OBJ_VAL(copyString(&res, 1));
}

Value StringIterator_Class(Value value) {
    return TYPE_CLASS(strIterator);
}