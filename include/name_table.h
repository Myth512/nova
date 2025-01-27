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

void initNameTable(NameTable *table);

void freeNameTable(NameTable *table);

bool nameTableSet(NameTable *table, ObjString *key, Value value);

bool nameTableGet(NameTable *table, ObjString *key, Value *value);

bool nameTableDelete(NameTable *table, ObjString *key);

void nameTableAddAll(NameTable *source, NameTable *destination);

void markNameTable(NameTable *table);

void printNameTable(NameTable *table);

#endif