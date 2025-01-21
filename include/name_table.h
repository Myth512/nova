#ifndef NAME_TABLE_H
#define NAME_TABLE_H

#include "value.h"

typedef struct {
    ObjString *key;
    Value value;
} NameEntry;

typedef struct {
    int size;
    int capacity;
    NameEntry *entries;
} NameTable;

void initTable(NameTable *table);

void freeTable(NameTable *table);

bool tableSet(NameTable *table, ObjString *key, Value value);

bool tableGet(NameTable *table, ObjString *key, Value *value);

bool tableDelete(NameTable *table, ObjString *key);

void tableAddAll(NameTable *source, NameTable *destination);

void markTable(NameTable *table);

void printTable(NameTable *table);

#endif