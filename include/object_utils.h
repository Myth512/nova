#ifndef OBJECT_UTILS_H
#define OBJECT_UTILS_H

#include "object.h"
#include "value.h"

void printObject(Value value);

int writeObject(Value value, char *buffer, const size_t maxSize);

bool compareObjects(Value a, Value b);

const char* decodeObjType(Value value);

const char *decodeObjTypeClean(Value value);

uint64_t hashObject(Value value);

bool objectEqual(Value a, Value b);

bool objectNotEqual(Value a, Value b);

bool objectGreater(Value a, Value b);

bool objectGreaterEqual(Value a, Value b);

bool objectLess(Value a, Value b);

bool objectLessEqual(Value a, Value b);

#endif