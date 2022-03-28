#include <math.h>
#include <string.h>

double split_bill(double base_amount, double tax_rate, double tip_rate, int num_people) {
	double cents = base_amount * (1 + tax_rate) * (1 + tip_rate) / num_people * 100;
	int rounded = (int) cents < cents ? cents + 1 : cents;
	return rounded / 100.0;
}


double adjust_price(double original_price) {
	return 10 * sqrt(original_price);
}

int sandy_eats(char menu_item[]) {
	int len = 0;
	int letters = 0;
	for (; menu_item[len]; len ++) {
		if ((menu_item[len] >= 'j' && menu_item[len] <= 'l') || (menu_item[len] >= 'J' && menu_item[len] <= 'L')) {
			return 0;
		}
		if ((menu_item[len] >= 'a' && menu_item[len] <= 'z') || (menu_item[len] >= 'A' && menu_item[len] <= 'Z')) {
			letters ++;
		}
	}
	return !(letters & 1 || strstr(menu_item, "fish") || strstr(menu_item, "Fish"));
}

// Assumes thing[] has enough space
void imagine_fish(char thing[]) {
	strcpy(thing + strlen(thing), "fish");
	
//	static const char *fish = "fish";
//	int len = 0, i = 0;
//	for(; thing[len]; len ++);	
//	for(; fish[i]; i ++) {
//		thing[len + i] = fish[i];
//	}
//	thing[len + i] = '\0';
}

