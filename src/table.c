#include "table.h"
#include "memory.h"
#include "value_methods.h"
#include "value_int.h"

#define MAX_LOAD_FACTOR 0.66

#define TOMBSTONE (Value){.type=UNDEFINED_VAL, .as.integer=1}
#define IS_TOMBSTONE(value) (value.type == UNDEFINED_VAL && value.as.integer == 1)

void QuadraticTableInit(Table *table) {
    table->size = 0;
    table->capacity = 0;
    table->entries = NULL;
}

void QuadraticTableFree(Table *table) {
    reallocate(table->entries, table->capacity * sizeof(Entry), 0);
    QuadraticTableInit(table);
}

static Entry *findEntry(Entry *entries, int capacity, Value key) {
    int j = 0;

    while (j < capacity) {
        int index = (valueHash(key) + j*j) % capacity;

        Entry *entry = &entries[index];

        if (IS_UNDEFINED(entry->key))
            return entry;

        if (AS_BOOL(valueEqual(key, entry->key)))
            return entry;

        j++;
    }

    return NULL;
}

static void resizeTable(Table *table, int capacity) {
    Entry *entries = reallocate(NULL, 0, sizeof(Entry) * capacity);

    for (int i = 0; i < capacity; i++) {
        entries[i].key = UNDEFINED_VAL;
        entries[i].value = UNDEFINED_VAL;
    }

    for (int i = 0; i < table->capacity; i++) {
        Entry *entry = &table->entries[i];

        if (IS_UNDEFINED(entry->key))
            continue;
        
        Entry *dest = findEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
    }

    FREE_VEC(Entry, table->entries, table->capacity);
    table->entries = entries;
    table->capacity = capacity;
}

Value QuadraticTableGet(Table *table, Value key) {
    Entry *entry = findEntry(table->entries, table->capacity, key);
    if (entry == NULL)
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
        table->size++;

    entry->key = key;
    entry->value = value;

    return isNew;
}

bool QuadraticTableDelete(Table *table, Value key) {

}