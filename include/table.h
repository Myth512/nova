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
    Entry **order;
} Table;

void tableInit(Table *table);

void tableFree(Table *table);

Value tableGet(Table *table, Value key);

bool tableSet(Table *table, Value key, Value value);

Value tableDelete(Table *table, Value key);

bool compareTables(Table *a, Table *b);

void tableDebug(Table *table);

#endif