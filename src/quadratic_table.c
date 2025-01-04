#include "quadratic_table.h"
#include "memory.h"
#include "value_methods.h"
#include "value_int.h"

#define MAX_LOAD_FACTOR 0.66

#define TOMBSTONE (Value){.type=UNDEFINED_VAL, .as.integer=1}
#define IS_TOMBSTONE(value) (value.type == UNDEFINED_VAL && value.as.integer == 1)

void QuadraticTableInit(QuadraticTable *table) {
    table->size = 0;
    table->capacity = 0;
    table->entries = NULL;
}

void QuadraticTableFree(QuadraticTable *table) {
    reallocate(table->entries, table->capacity * sizeof(QuadraticEntry), 0);
    QuadraticTableInit(table);
}

static QuadraticEntry *findEntry(QuadraticEntry *entries, int capacity, Value key) {
    int j = 0;

    while (j < capacity) {
        int index = (valueHash(key) + j*j) % capacity;

        QuadraticEntry *entry = &entries[index];

        if (IS_UNDEFINED(entry->key))
            return entry;

        if (AS_BOOL(valueEqual(key, entry->key)))
            return entry;

        j++;
    }

    return NULL;
}

static void resizeTable(QuadraticTable *table, int capacity) {
    QuadraticEntry *entries = reallocate(NULL, 0, sizeof(QuadraticEntry) * capacity);

    for (int i = 0; i < capacity; i++) {
        entries[i].key = UNDEFINED_VAL;
        entries[i].value = UNDEFINED_VAL;
    }

    for (int i = 0; i < table->capacity; i++) {
        QuadraticEntry *entry = &table->entries[i];

        if (IS_UNDEFINED(entry->key))
            continue;
        
        QuadraticEntry *dest = findEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
    }

    FREE_VEC(QuadraticEntry, table->entries, table->capacity);
    table->entries = entries;
    table->capacity = capacity;
}

Value QuadraticTableGet(QuadraticTable *table, Value key) {
    QuadraticEntry *entry = findEntry(table->entries, table->capacity, key);
    if (entry == NULL)
        return UNDEFINED_VAL;
    return entry->value;
}

bool QuadraticTableSet(QuadraticTable *table, Value key, Value value) {
    if (table->size + 1 >= table->capacity * MAX_LOAD_FACTOR) {
        resizeTable(table, GROW_CAPACITY(table->capacity));
    }

    QuadraticEntry *entry = findEntry(table->entries, table->capacity, key);

    bool isNew = IS_UNDEFINED(entry->key);

    if (isNew)
        table->size++;

    entry->key = key;
    entry->value = value;

    return isNew;
}

bool QuadraticTableDelete(QuadraticTable *table, Value key) {

}