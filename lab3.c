#include "lab3.h"
#include <string.h>

//Add TA party item request to the list
int add_request(struct party_node **head, char *item, double price, char *ta){
	if (!strcmp(item, "IDE"))
		return -1;
	struct party_node *node = (struct party_node *) malloc(sizeof(struct party_node));
	if (!node)
		return -1;
	node->item = (char *) malloc(strlen(item) + 1);
	if (!node->item) {
		free(node);
		return -1;
	}
	node->ta = (char *) malloc(strlen(ta) + 1);
	if (!node->ta) {
		free(node->item);
		free(node);
		return -1;
	}

	strcpy(node->item, item);
	strcpy(node->ta, ta);
	node->price = price;

	node->next = *head;
	*head = node;
	return 0;
}

//Remove the last item added
void remove_request(struct party_node **head){
	struct party_node *node = *head;
	*head = (*head)->next;
	free(node->item);
	free(node->ta);
	free(node);
}

struct party_node* merge(struct party_node *a, struct party_node *b) {
	if (!a)
		return b;
	if (!b)
		return a;
	struct party_node *node;
	if (b->price > a->price) {
		node = b;
		b = b->next;
	}
	else {
		node = a;
		a = a->next;
	}
	struct party_node *out = node;
	while (a && b) {
		if (b->price > a->price) {
			node->next = b;
			b = b->next;
		}
		else {
			node->next = a;
			a = a->next;
		}
		node = node->next;
	}
	while (a) {
		node->next = a;
		a = a->next;
		node = node->next;
	}
	while (b) {
		node->next = b;
		b = b->next;
		node = node->next;
	}
	node->next = NULL;
	return out;
}

struct party_node* split(struct party_node *list) {
	if (!list)
		return NULL;
	struct party_node *fast = list, *slow = list;
	struct party_node *out = slow;
	while (fast) {
		fast = fast->next;
		if (!fast)
			break;
		fast = fast->next;

		out = slow;
		slow = slow->next;
	}
	out->next = NULL;
	return slow;
}

//Sort party item requests - in place?
void make_sorted(struct party_node **head){
	// Base case: empty list or list of 1 element
	if (!*head || !(*head)->next)
		return;
	struct party_node *a = *head, *b;
	b = split(a);
	make_sorted(&a);
	make_sorted(&b);
	*head = merge(a, b);
}

//Trim list to fit the budget
double finalize_list(struct party_node **head, double budget){
	if (!*head)
		return budget;
	if ((*head)->price > budget) {
		remove_request(head);
		return finalize_list(head, budget);
	}
	budget -= (*head)->price;
	return finalize_list(&(*head)->next, budget);

//	struct party_node dummy = { NULL, 0, NULL, *head };
//	struct party_node *node = &dummy;
//	while (node->next && budget) {
//		while (node->next && node->next->price > budget) {
//			remove_request(&node->next);
//		}
//		if (!node->next)
//			break;
//		budget -= node->next->price;
//		node = node->next;
//	}
//	*head = dummy.next;
//	return budget;
}

//Print the current list - hope this is helpful!
void print_list(struct party_node *head){
    int count = 1;
    printf("The current list contains:\n");
    while(head!=NULL){
        printf("Item %d: %s, %.2lf, requested by %s\n",
            count, head->item, head->price, head->ta);
        count++;
        head = head->next;
    }
    printf("\n\n");
    return;
}
