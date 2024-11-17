#ifndef TABLE_H
#define TABLE_H

#include "value.h"

typedef struct {
    ObjString *key;
    Value value;
} Entry;

typedef struct {
    int size;
    int capacity;
    Entry *entries;
} Table;

void initTable(Table *table);

void freeTable(Table *table);

bool tableSet(Table *table, ObjString *key, Value value);

bool tableGet(Table *table, ObjString *key, Value *value);

bool tableDelete(Table *table, ObjString *key);

void tableAddAll(Table *source, Table *destination);

ObjString* tableFindString(Table *table, const char *chars, int length, uint32_t hash);

void markTable(Table *table);

#endif