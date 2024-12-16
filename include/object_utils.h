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

#endif