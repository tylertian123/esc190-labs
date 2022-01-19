#include <stdio.h>
#include "lab1.h"

int main() {
	double test_p1 = split_bill(50.01, 0.13, 0.15, 2);
	printf("Split: $%.2f\n", test_p1);
	double test_p2 = split_bill(50.01, 0, 0, 2);
	printf("Split: $%.2f\n", test_p2);
	double test_p3 = split_bill(50.00, 0, 0, 2);
	printf("Split: $%.2f\n", test_p3);

	char test_food[] = "Three Cheese Pizza";
	printf("Sandy would%s eat '%s'.\n", sandy_eats(test_food) ? "" : " NOT", test_food);

	char test_p4[30] = "zebra";
	imagine_fish(test_p4);
	printf("Imagine %s\n", test_p4);

	return 0;
}

