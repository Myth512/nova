#ifndef VALUE_UTILS_H
#define VALUE_UTILS_H

#include "value.h"

int writeToBuffer(char *buffer, const size_t size, const char *format, ...);

int valueWrite(Value value, char *buffer, const size_t size);

int valuePrint(Value value);

Value valueEqual(Value a, Value b);

Value valueNotEqual(Value a, Value b);

Value valueGreater(Value a, Value b);

Value valueGreaterEqual(Value a, Value b);

Value valueLess(Value a, Value b);

Value valueLessEqual(Value a, Value b);

Value valueAdd(Value a, Value b);

Value valueSubtract(Value a, Value b);

Value valueMultiply(Value a, Value b);

Value valueTrueDivide(Value a, Value b);

Value valueFloorDivide(Value a, Value b);

Value valueModulo(Value a, Value b);

Value valuePower(Value a, Value b);

Value valuePlus(Value a);

Value valueNegate(Value a);

Value valueAnd(Value a, Value b);

Value valueXor(Value a, Value b);

Value valueOr(Value a, Value b);

Value valueInvert(Value a);

Value valueLeftShift(Value a, Value b);

Value valueRightShift(Value a, Value b);

OptValue valueGetAttribute(Value obj, ObjString *name);

void valueSetAttribute(Value obj, ObjString *name, Value value);

void valueDelAttribute(Value obj, ObjString *name);

Value valueGetItem(Value obj, Value key);

void valueSetItem(Value obj, Value key, Value value);

void valueDelItem(Value obj, Value key);

Value valueCall(Value obj);

uint64_t valueHash(Value value);

uint64_t valueId(Value value);

int valueLen(Value value);

bool valueToBool(Value value);

long long valueToInt(Value value);

double valueToFloat(Value value);

ObjString *valueToStr(Value value);

ObjString *valueToRepr(Value value);

#endif