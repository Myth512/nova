#ifndef VALUE_UTILS_H
#define VALUE_UTILS_H

#include "value.h"

int writeToBuffer(char *buffer, const size_t size, const char *format, ...);

int valueWrite(Value value, char *buffer, const size_t size);

int valueReprWrite(Value value, char *buffer, size_t size);

int valuePrint(Value value);

int valueRepr(Value value);

char *getValueType(Value value);

Value valueIs(Value a, Value b);

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

Value valuePositive(Value a);

Value valueNegative(Value a);

Value valueAnd(Value a, Value b);

Value valueXor(Value a, Value b);

Value valueOr(Value a, Value b);

Value valueInvert(Value a);

Value valueLeftShift(Value a, Value b);

Value valueRightShift(Value a, Value b);

Value valueContains(Value a, Value b);

Value valueGetAttr(Value obj, ObjString *name);

void valueSetAttr(Value obj, ObjString *name, Value value);

void valueDelAttr(Value obj, ObjString *name);

Value valueGetItem(Value obj, Value key);

Value valueSetItem(Value obj, Value key, Value value);

Value valueDelItem(Value obj, Value key);

Value valueInit(Value callee, int argc, Value *argv);

Value valueCall(Value callee, int argc, int kwargc, Value *argv);

Value valueClass(Value value);

Value valueIter(Value value);

Value valueNext(Value value);

uint64_t valueHash(Value value);

uint64_t valueId(Value value);

long long valueLen(Value value);

bool valueToBool(Value value);

long long valueToInt(Value value);

double valueToFloat(Value value);

ObjString *valueToStr(Value value);

ObjString *valueToRepr(Value value);

bool isInstance(Value obj, Value class);

#endif