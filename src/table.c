#include <stdio.h>

#include "table.h"
#include "memory.h"
#include "value_methods.h"
#include "value_int.h"

#define MAX_LOAD_FACTOR 0.66

#define TOMBSTONE ((Value){.type=VAL_UNDEFINED, .as.integer=1})
#define IS_TOMBSTONE(value) (value.type == VAL_UNDEFINED && value.as.integer == 1)

void QuadraticTableInit(Table *table) {
    table->size = 0;
    table->capacity = 0;
    table->entries = NULL;
    table->order = NULL;
}

void QuadraticTableFree(Table *table) {
    reallocate(table->entries, table->capacity * sizeof(Entry), 0);
    QuadraticTableInit(table);
}

static Entry *findEntry(Entry *entries, int capacity, Value key) {
    uint64_t hash = valueHash(key);
    int step = 7 - (hash % 7);
    Entry *tombstone = NULL;

    for (int i = 0; i < capacity; i++) {
        int index = (hash + i * step) % capacity;

        Entry *entry = &entries[index];

        if (IS_TOMBSTONE(entry->key)) {
            if (tombstone == NULL)
                tombstone = entry;
        } else if (IS_UNDEFINED(entry->key))
            return tombstone == NULL ? entry : tombstone;
        else if (AS_BOOL(valueEqual(key, entry->key)))
            return entry;
    }
    return NULL;
}

static void resizeTable(Table *table, int capacity) {
    Entry *entries = reallocate(NULL, 0, sizeof(Entry) * capacity);
    Entry **order = reallocate(NULL, 0, sizeof(Entry*) * capacity);

    for (int i = 0; i < capacity; i++) {
        entries[i].key = UNDEFINED_VAL;
        entries[i].value = UNDEFINED_VAL;
    }

    for (int i = 0; i < table->size; i++) {
        Entry *entry = table->order[i]; 

        if (IS_UNDEFINED(entry->key))
            continue;
        
        Entry *dest = findEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        order[i] = dest;
    }

    FREE_VEC(Entry, table->entries, table->capacity);
    FREE_VEC(Entry*, table->order, table->capacity);
    table->entries = entries;
    table->order = order;
    table->capacity = capacity;
}

Value QuadraticTableGet(Table *table, Value key) {
    Entry *entry = findEntry(table->entries, table->capacity, key);
    if (entry == NULL || IS_TOMBSTONE(entry->key))
        return UNDEFINED_VAL;
    return entry->value;
}

bool QuadraticTableSet(Table *table, Value key, Value value) {
    if (table->size + 1 >= table->capacity * MAX_LOAD_FACTOR) {
        resizeTable(table, GROW_CAPACITY(table->capacity));
    }

    Entry *entry = findEntry(table->entries, table->capacity, key);

    bool isNew = IS_UNDEFINED(entry->key);

    if (isNew)
        table->order[table->size++] = entry;

    entry->key = key;
    entry->value = value;

    return isNew;
}

static void removeFromOrder(Table *table, Entry *entry) {
    int i = 0;
    for (; table->order[i] != entry; i++);

    for (; i < table->size - 1; i++)
        table->order[i] = table->order[i + 1];
}

bool QuadraticTableDelete(Table *table, Value key) {
    if (table->size == 0)
        return false;

    Entry *entry = findEntry(table->entries, table->capacity, key);

    if (IS_UNDEFINED(entry->key))
        return false;
    
    removeFromOrder(table, entry);
    
    entry->key = TOMBSTONE;
    entry->value = UNDEFINED_VAL;
    table->size--;
    return true;
}

void TableDebug(Table *table) {
    printf("size: %d, capacity: %d\n", table->size, table->capacity);

    for (int i = 0; i < table->size; i++) {
        Entry *entry = table->order[i];
        printf("%2d | ", i);
        if (IS_TOMBSTONE(entry->key))
            printf("tombstone");
        else
            valueRepr(entry->key);
        printf(" : ");
        valueRepr(entry->value);
        printf("\n");
    }
}