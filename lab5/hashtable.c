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

void hashtable_insert(HashTable *table, HashTableKey key, HashTableValue value) {
    unsigned int hash = hashtable_hash(key);
    
    if (!table->entries) {
        // Start with 64 entries
        table->count = 0;
        table->buckets = 64;
        table->entries = (HashTableEntry **) calloc(table->buckets, sizeof(HashTableEntry*));
    }
    
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
}

HashTableValue hashtable_retrieve(HashTable *table, HashTableKey key) {
    unsigned int hash = hashtable_hash(key);
    unsigned int index = hash % table->buckets;
    HashTableEntry *entry = table->entries[index];
    while (entry) {
        if (hashtable_compare_key(entry->key, key)) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL; // Undefined behaviour
}

#include <stdio.h>

int main() {
    HashTable table = {NULL, 0, 0};
    hashtable_insert(&table, "a", 1);
    hashtable_insert(&table, "b", 2);
    hashtable_insert(&table, "c", 3);
    hashtable_insert(&table, "d", 4);

    printf("key %s value %d\n", "a", hashtable_retrieve(&table, "a"));
    printf("key %s value %d\n", "c", hashtable_retrieve(&table, "c"));
    printf("key %s value %d\n", "d", hashtable_retrieve(&table, "d"));
}
