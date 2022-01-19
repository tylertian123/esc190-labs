#include <stdlib.h>

// Regardig parameters:
// If int **x -> int *x, the function can still work if the return type can be int*, otherwise it becomes impossible
// Similary for int *n -> int n, although incrementing n outside the function is trivial
void append(int **x, int *n, int val) {
	(*n) ++;
	// If realloc() fails we get a memory leak but not before a segmentation fault, yay
	*x = (int *) realloc(*x, sizeof(int) * (*n));
	(*x)[*n - 1] = val;
}

