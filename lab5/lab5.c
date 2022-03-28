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