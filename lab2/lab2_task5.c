#include <stdio.h>
#include <stdlib.h>

int main() {
	int *p = (int *) malloc(sizeof(int));
	int *q = (int *) malloc(sizeof(int) * 2);

	*p = 1;
	*q = 10;

	q = p; // Oh no
	free(q);
	free(p); // q freed twice, p never freed

	return 0;
}

