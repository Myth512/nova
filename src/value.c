#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "value.h"
#include "object.h"
#include "object_string.h"
#include "object_class.h"
#include "memory.h"
#include "object_exception.h"
#include "error.h"
#include "vm.h"

int writeToBuffer(char *buffer, const size_t size, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int bytesWritten;

    if (buffer == NULL)
        bytesWritten = vprintf(format, args);
    else
        bytesWritten = vsnprintf(buffer, size, format, args);
    
    va_end(args);

    return bytesWritten;
}

Value getStaticAttribute(Value value, ObjString *name, const struct StaticAttribute*(*in_word_set)(register const char*, register size_t)) {
    const StaticAttribute *result = in_word_set(name->chars, name->length);
    if (!result)
        return createException(VAL_ATTRIBUTE_ERROR, "'%s' object has no attribute '%s'", getValueType(value), name->chars);
    if (result->isMethod)
        return OBJ_VAL(createNativeMethod(value, result->as.method, result->name));
    return result->as.attribue(value);
}

int calculateIndex(int index, int length) {
    if (index < -length)
        return -1;
    
    if (index >= length)
        return -2;
    
    if (index >= 0)
        return index;
    
    return length + index;
}

int movePointer(char **buffer, int bytesWritten) {
    if (*buffer != NULL) {
        *buffer += bytesWritten;
        return bytesWritten;
    }
    return 0;
}