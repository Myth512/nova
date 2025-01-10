#ifndef QUADRATIC_TABLE_H
#define QUADRATIC_TABLE_H

#include <stdlib.h>

#include "value.h"

typedef struct {
    Value key;
    Value value;
} Entry;

typedef struct {
    size_t size;
    size_t capacity;
    Entry *entries;
} Table;

void QuadraticTableInit(Table *table);

void QuadraticTableFree(Table *table);

Value QuadraticTableGet(Table *table, Value key);

bool QuadraticTableSet(Table *table, Value key, Value value);

bool QuadraticTableDelete(Table *table, Value key);

void TableDebug(Table *table);

#endif