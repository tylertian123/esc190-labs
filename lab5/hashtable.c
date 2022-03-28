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
    
    if (!table->entries) {
        // Start with 64 entries
        table->count = 0;
        table->buckets = 4;
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
            hashtable_insert(table, e->key, e->value);
            e = e->next;
        }
    }
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
    return 0;
}

#include <stdio.h>

int main() {
    HashTable table = {NULL, 0, 0};
    
    hashtable_insert(&table, "a", 1);
    printf("Table has %d elements in %d buckets\n", table.count, table.buckets);
    hashtable_insert(&table, "b", 2);
    hashtable_insert(&table, "c", 3);
    hashtable_insert(&table, "d", 4);
    printf("Table has %d elements in %d buckets\n", table.count, table.buckets);
    hashtable_insert(&table, "e", 5);
    hashtable_insert(&table, "f", 6);
    hashtable_insert(&table, "g", 7);
    hashtable_insert(&table, "h", 8);
    hashtable_insert(&table, "i", 9);
    printf("Table has %d elements in %d buckets\n", table.count, table.buckets);

    printf("key %s value %d\n", "e", hashtable_retrieve(&table, "e"));
    printf("key %s value %d\n", "f", hashtable_retrieve(&table, "f"));
    printf("key %s value %d\n", "g", hashtable_retrieve(&table, "g"));
}
