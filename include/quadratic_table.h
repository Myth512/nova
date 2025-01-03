#ifndef QUADRATIC_TABLE_H
#define QUADRATIC_TABLE_H

#include <stdlib.h>

#include "value.h"

typedef struct {
    Value key;
    Value value;
} QuadraticEntry;

typedef struct {
    size_t size;
    size_t capacity;
    QuadraticEntry *entries;
} QuadraticTable;

void QuadraticTableInit(QuadraticTable *table);

void QuadraticTableFree(QuadraticTable *table);

Value QuadraticTableGet(QuadraticTable *table, Value key);

bool QuadraticTableSet(QuadraticTable *table, Value key, Value value);

bool QuadraticTableDelete(QuadraticTable *table, Value key);

#endif