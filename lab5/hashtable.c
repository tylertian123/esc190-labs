#include <stdlib.h>
#include <string.h>

typedef char *HashTableKey;
typedef int HashTableValue;


typedef struct HashTableEntry {
    HashTableKey key;
    HashTableValue value;
    struct HashTableEntry *next;
} HashTableEntry;

typedef struct HashTable {
    HashTableEntry **entries;
    int count;
    int buckets;
} HashTable;

unsigned int hashtable_hash(HashTableKey key) {
    static const unsigned int m = 613651349;
    static const unsigned int p = 53;

    unsigned int hash = 0;
    unsigned int hash_multiplier = 1;
    for (int i = 0; key[i]; i ++) {
        hash += key[i] * hash_multiplier % m;
        hash_multiplier *= p;
    }
    return hash;
}

int hashtable_compare_key(HashTableKey a, HashTableKey b) {
    return !strcmp(a, b);
}

void hashtable_resize(HashTable *table, unsigned int new_size);

void hashtable_insert(HashTable *table, HashTableKey key, HashTableValue value) {

    unsigned int hash = hashtable_hash(key);
    
    unsigned int index = hash % table->buckets;
    // Replace if already exists
    HashTableEntry *entry = table->entries[index];
    while (entry) {
        if (hashtable_compare_key(entry->key, key)) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }
    // Insert if it does not exist
    HashTableEntry *new_entry = (HashTableEntry *) malloc(sizeof(HashTableEntry));
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = table->entries[index];
    table->entries[index] = new_entry;
    table->count ++;

    if (table->count * 10 > table->buckets * 8) {
        hashtable_resize(table, table->buckets * 2);
    }
}

void hashtable_resize(HashTable *table, unsigned int new_size) {
    HashTableEntry **old_entries = table->entries;
    int buckets = table->buckets;

    table->buckets = new_size;
    table->count = 0;
    table->entries = (HashTableEntry **) calloc(table->buckets, sizeof(HashTableEntry *));

    for (int i = 0; i < buckets; i ++) {
        HashTableEntry *e = old_entries[i];
        while (e) {
            HashTableEntry *next = e->next;
            hashtable_insert(table, e->key, e->value);
            free(e);
            e = next;
        }
    }
    free(old_entries);
}

HashTableValue hashtable_retrieve(HashTable *table, HashTableKey key, int *contains_out) {
    unsigned int hash = hashtable_hash(key);
    unsigned int index = hash % table->buckets;
    HashTableEntry *entry = table->entries[index];
    while (entry) {
        if (hashtable_compare_key(entry->key, key)) {
            if (contains_out) {
                *contains_out = 1;
            }
            return entry->value;
        }
        entry = entry->next;
    }
    if (contains_out) {
        *contains_out = 0;
    }
    return 0;
}

HashTableValue hashtable_delete(HashTable *table, HashTableKey key) {
    unsigned int hash = hashtable_hash(key);
    unsigned int index = hash % table->buckets;
    HashTableEntry *entry = table->entries[index], *parent = NULL;
    while (entry) {
        if (hashtable_compare_key(entry->key, key)) {
            break;
        }
        parent = entry;
        entry = entry->next;
    }
    // Delete entry if it exists
    if (entry) {
        HashTableValue retval = entry->value;
        if (!parent) {
            table->entries[index] = entry->next;
        }
        else {
            parent->next = entry->next;
        }
        free(entry);
        return retval;
    }
    return 0;
}

HashTable* hashtable_init(int buckets) {
    HashTable *table = (HashTable *) malloc(sizeof(HashTable));
    table->count = 0;
    table->buckets = buckets;
    table->entries = (HashTableEntry **) calloc(table->buckets, sizeof(HashTableEntry*));
    return table;
}

void hashtable_free(HashTable *table) {
    for (int i = 0; i < table->buckets; i++) {
        HashTableEntry *e = table->entries[i];
        while (e) {
            HashTableEntry *temp = e->next;
            free(e);
            e = temp;
        }
    }
    free(table->entries);
    free(table);
}

#include <stdio.h>

int main() {
    HashTable *table = hashtable_init(1);
    
    hashtable_insert(table, "a", 1);
    printf("Table has %d elements in %d buckets\n", table->count, table->buckets);
    hashtable_insert(table, "b", 2);
    hashtable_insert(table, "c", 3);
    hashtable_insert(table, "d", 4);
    printf("Table has %d elements in %d buckets\n", table->count, table->buckets);
    hashtable_insert(table, "e", 5);
    hashtable_insert(table, "f", 6);
    hashtable_insert(table, "g", 7);
    hashtable_insert(table, "h", 8);
    hashtable_insert(table, "i", 9);
    printf("Table has %d elements in %d buckets\n", table->count, table->buckets);

    printf("Deleting key %s (value %d)\n", "a", hashtable_delete(table, "a"));
    int contains;
    hashtable_retrieve(table, "a", &contains);
    printf("Table contains %s?: %d\n", "a", contains);

    printf("key %s value %d\n", "e", hashtable_retrieve(table, "e", NULL));
    printf("key %s value %d\n", "f", hashtable_retrieve(table, "f", NULL));
    printf("key %s value %d\n", "g", hashtable_retrieve(table, "g", NULL));

    hashtable_free(table);
}
