#include "lab5.h"

#define INT_MAX 0x7FFFFFFF

typedef struct {
    int cost;
    Vnode *prev;
} HeapElement;

typedef struct {
    HeapElement *elems;
    int count;
    int capacity;
} Heap;

/*
 * Return a nonzero value if a has higher priority than b.
 */
int heap_compare(HeapElement a, HeapElement b) {
    return a.cost < b.cost;
}

/*
 * Restore heap property by sifting up from the last element.
 */
void heap_sift_up(Heap *heap) {
    int i = heap->count - 1;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap_compare(heap->elems[i], heap->elems[parent])) {
            HeapElement temp = heap->elems[parent];
            heap->elems[parent] = heap->elems[i];
            heap->elems[i] = temp;
        }
        else {
            break;
        }
    }
}

/*
 * Insert e into the heap.
 */
void heap_insert(Heap *heap, HeapElement e) {
    if (!heap->elems) {
        heap->elems = malloc(sizeof(HeapElement));
        heap->elems[0] = e;
        heap->capacity = heap->count = 1;
        return;
    }
    heap->count ++;
    if (heap->count > heap->capacity) {
        heap->capacity *= 2;
        heap->elems = realloc(heap->elems, heap->capacity * sizeof(HeapElement));
    }
    heap->elems[heap->count - 1] = e;
    heap_sift_up(heap);
}

/*
 * Restore heap property by sifting down from the first element.
 */
void heap_sift_down(Heap* heap) {
    int i = 0;
    while (i < heap->count) {
        int lc = i * 2 + 1, rc = lc + 1;
        // Both children
        if (rc < heap->count) {
            int min_child = heap_compare(heap->elems[lc], heap->elems[rc]) ? lc : rc;
            if (heap_compare(heap->elems[min_child], heap->elems[i])) {
                HeapElement temp = heap->elems[i];
                heap->elems[i] = heap->elems[min_child];
                heap->elems[min_child] = temp;
                i = min_child;
            }
            else {
                break;
            }
        }
        // Only one child
        else if (lc < heap->count && heap_compare(heap->elems[lc], heap->elems[i])) {
            HeapElement temp = heap->elems[i];
            heap->elems[i] = heap->elems[lc];
            heap->elems[lc] = temp;
            i = lc;
        }
        // Leaves
        else {
            break;
        }
    }
}

/*
 * Remove and return the highest priority element in the heap.
 * Assume heap is nonempty.
 */
HeapElement heap_remove(Heap *heap) {
    HeapElement e = heap->elems[0];
    heap->count --;
    heap->elems[0] = heap->elems[heap->count];
    heap_sift_down(heap);
    return e;
}

Heap* heap_init() {
    Heap *heap = (Heap*) malloc(sizeof(Heap));
    heap->count = heap->capacity = 0;
    heap->elems = NULL;
    return heap;
}

void heap_free(Heap *heap) {
    if (heap->elems) {
        free(heap->elems);
    }
    free(heap);
}


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
            hashtable_insert(table, e->key, e->value);
            e = e->next;
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

char **plan_route(Graph *gr, char *start, char *dest){
    //Add code here
}

void add(Graph *gr, char *station){
    for (int i = 0; i < gr->count; i ++) {
        if (!strcmp(gr->adj_list[i]->station, station)) {
            return;
        }
    }
    Vnode *node = (Vnode *) malloc(sizeof(Vnode));
    strcpy(node->station, station);
    node->edges = NULL;
    node->cost = INT_MAX;
    node->visited = 0;
    node->prev = NULL;

    gr->count ++;
    gr->adj_list = realloc(gr->adj_list, gr->count * sizeof(Vnode *));
    gr->adj_list[gr->count - 1] = node;
}

void update(Graph *gr, char *start, char *dest, int weight){
    Vnode *s = NULL, *e = NULL;
    // Look for start and end node
    for (int i = 0; i < gr->count; i ++) {
        if (!strcmp(gr->adj_list[i]->station, start)) {
            s = gr->adj_list[i];
        }
        if (!strcmp(gr->adj_list[i]->station, dest)) {
            e = gr->adj_list[i];
        }
    }
    // Add missing nodes
    if (!s) {
        if (!weight) {
            return;
        }
        add(gr, start);
        s = gr->adj_list[gr->count - 1];
    }
    if (!e) {
        if (!weight) {
            return;
        }
        add(gr, dest);
        e = gr->adj_list[gr->count - 1];
    }

    // Update existing
    Enode *edge = s->edges, *prev = NULL;
    while (edge) {
        // Find the edge
        if (!strcmp(edge->vertex, dest)) {
            // Remove existing edge
            if (!weight) {
                // Head
                if (edge == s->edges) {
                    s->edges = s->edges->next;
                }
                else {
                    prev->next = edge->next;
                }
                free(edge);
                return;
            }
            else {
                edge->weight = weight;
                return;
            }
        }

        prev = edge;
        edge = edge->next;
    }
    // No existing edge
    if (!weight) {
        return;
    }
    // Create new
    Enode *new_edge = (Enode *) malloc(sizeof(Enode));
    strcpy(new_edge->vertex, dest);
    new_edge->weight = weight;
    new_edge->next = s->edges;
    s->edges = new_edge;
}

void disrupt(Graph *gr, char *station){
    //Add code here
}