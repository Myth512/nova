#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "name_table.h"
#include "memory.h"
#include "value_methods.h"
#include "value_int.h"
#include "object_string.h"

#define TABLE_MAX_LOAD 0.75

void initNameTable(NameTable *table) {
    table->size = 0;
    table->capacity = 0;
    table->entries = NULL;
}

void freeNameTable(NameTable *table) {
    FREE_VEC(NameEntry, table->entries, table->capacity);
    initNameTable(table);
}

static NameEntry* findEntry(NameEntry *entries, int capacity, ObjString *key) {
    uint32_t index = String_Hash(STRING_VAL(key)) % capacity; 
    NameEntry *tombstone = NULL;

    while (true) {
        NameEntry *entry = &entries[index];
        if (entry->key == NULL) {
            if (IS_NONE(entry->value)) {
                return tombstone != NULL ? tombstone : entry;
            } else {
                if (tombstone == NULL)
                    tombstone = entry;
            }
        } else if (valueToBool(String_Equal(STRING_VAL(key), STRING_VAL(entry->key)))) {
            return entry;
        }
        
        index = (index + 1) % capacity;
    }
}

static void adjustCapacity(NameTable *table, int capacity) {
    NameEntry *entries = ALLOCATE(NameEntry, capacity);
    for (int i = 0; i < capacity; i++) {
        entries[i].key = NULL;
        entries[i].value = NONE_VAL;
    }

    table->size = 0; 
    for (int i = 0; i < table->capacity; i++) {
        NameEntry *entry = &table->entries[i];
        if (entry->key == NULL)
            continue;

        NameEntry *dest = findEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        table->size++;
    }

    FREE_VEC(NameEntry, table->entries, table->capacity);
    table->entries = entries;
    table->capacity = capacity;
}

bool nameTableSet(NameTable *table, ObjString *key, Value value) {
    if (table->size + 1 > table->capacity * TABLE_MAX_LOAD) {
        int capacity = GROW_CAPACITY(table->capacity);
        adjustCapacity(table, capacity);
    }

    NameEntry *entry = findEntry(table->entries, table->capacity, key);
    bool isNewKey = entry->key == NULL;
    if (isNewKey && IS_NONE(entry->value))
        table->size++;

    entry->key = key;
    entry->value = value;
    return isNewKey;
}

bool nameTableGet(NameTable *table, ObjString *key, Value *value) {
    if (table->size == 0)
        return false;
    
    NameEntry *entry = findEntry(table->entries, table->capacity, key);
    if (entry->key == NULL)
        return false;
    
    *value = entry->value;
    return true;
}

bool nameTableDelete(NameTable *table, ObjString *key) {
    if (table->size == 0)
        return false;

    NameEntry *entry = findEntry(table->entries, table->capacity, key);
    if (entry->key == NULL) 
        return false;
    
    entry->key = NULL;
    entry->value = BOOL_VAL(true);
    return true;
}

void nameTableAddAll(NameTable *source, NameTable *destination) {
    for (int i = 0; i < source->capacity; i++) {
        NameEntry *entry = &source->entries[i];
        if (entry->key != NULL)
            nameTableSet(destination, entry->key, entry->value);
    }
}

// ObjString* tableFindString(NameTable *table, const char *chars, int length, uint32_t hash) {
//     if (table->size == 0)
//         return NULL;

//     uint32_t index = hash % table->capacity;

//     while (true) {
//         NameEntry *entry = &table->entries[index];
//         if (entry->key == NULL) {
//             if (IS_NIL(entry->value))
//                 return NULL;
//         } else if (entry->key->length == length &&
//                    entry->key->hash == hash &&
//                    memcmp(entry->key->chars, chars, length) == 0) {
//             return entry->key;
//         }

//         index = (index + 1) % table->capacity;
//     }
// }

void markNameTable(NameTable *table) {
    for (int i = 0; i < table->capacity; i++) {
        NameEntry *entry = &table->entries[i];
        markObject((Obj*)entry->key);
        markValue(entry->value);
    }
}