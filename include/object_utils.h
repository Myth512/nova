#ifndef OBJECT_UTILS_H
#define OBJECT_UTILS_H

#include "object.h"
#include "value.h"

const char* decodeObjType(Value value);

const char *decodeObjTypeClean(Value value);

int objectWrite(Value value, char *buffer, const size_t size);

int objectPrint(Value value);

bool objectEqual(Value a, Value b);

bool objectNotEqual(Value a, Value b);

bool objectGreater(Value a, Value b);

bool objectGreaterEqual(Value a, Value b);

bool objectLess(Value a, Value b);

bool objectLessEqual(Value a, Value b);

Value objectAdd(Value a, Value b);

Value objectSubtract(Value a, Value b);

Value objectMultiply(Value a, Value b);

Value objectDivide(Value a, Value b);

Value objectModulo(Value a, Value b);

Value objectPower(Value a, Value b);

Value objectNegate(Value a);

Value objectIncrement(Value a);

Value objectDecrement(Value a);

OptValue objectGetField(Value obj, ObjString *name);

void objectSetField(Value obj, ObjString *name, Value value);

Value objectGetAt(Value obj, Value key);

void objectSetAt(Value obj, Value key, Value value);

uint64_t objectHash(Value a);

int objectLen(Value value);

bool objectToBool(Value value);

int objectToInt(Value value);

double objectToFloat(Value value);

#endif