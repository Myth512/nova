#ifndef OBJECT_UTILS_H
#define OBJECT_UTILS_H

#include "object.h"
#include "value.h"

void printObject(Value value);

int writeObject(Value value, char *buffer, const size_t maxSize);

const char* decodeObjType(Value value);

const char *decodeObjTypeClean(Value value);

Value objectEqual(Value a, Value b);

Value objectNotEqual(Value a, Value b);

Value objectGreater(Value a, Value b);

Value objectGreaterEqual(Value a, Value b);

Value objectLess(Value a, Value b);

Value objectLessEqual(Value a, Value b);

Value objectNot(Value a);

Value objectAdd(Value a, Value b);

Value objectSubtract(Value a, Value b);

Value objectMultiply(Value a, Value b);

Value objectDivide(Value a, Value b);

Value objectModulo(Value a, Value b);

Value objectPower(Value a, Value b);

Value objectNegate(Value a);

Value objectIncrement(Value a);

Value objectDecrement(Value a);

#endif