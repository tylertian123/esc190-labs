#include "lab5.h"

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
    //Add code here
}

void update(Graph *gr, char *start, char *dest, int weight){
    //Add code here
}

void disrupt(Graph *gr, char *station){
    //Add code here
}