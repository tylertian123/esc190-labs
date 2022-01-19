#include <stdio.h>
#include <stdlib.h>

void append(int **x, int *n, int val);

void print_arr(int *arr, int n) {
	for (int i = 0; i < n; i ++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int main() {
	int n = 5;
	int *arr = (int *) malloc(sizeof(int) * n);
	for (int i = 0; i < n; i ++) {
		arr[i] = i;
	}

	printf("Length before append: %d, values before append:\n", n);
	print_arr(arr, n);

	append(&arr, &n, n);

	printf("Length after append: %d, values after append:\n", n);
	print_arr(arr, n);

	free(arr);
}

