#include <stdio.h>

struct llist_node {
	int data;
	struct llist_node *prev;
	struct llist_node *next;
};

struct llist {
	struct llist_node *begin;
	struct llist_node *end;
};



struct llist *llist_create()
{
	struct llist *ll = malloc(sizeof(struct llist));
	if(!ll) {
		printf("ERROR: Memory error");
		exit(1);
	}

	ll->begin = NULL;
	ll->end = NULL;

	return ll;
}

void llist_insert_before(int data, struct llist_node node)
{
	if(!node) {
		return;
	}

	struct llist_node n_new = malloc(sizeof(struct llist_node));
	if(!n_new) {
		printf("ERROR: Memory error");
		exit(1);
	}

	n_new->prev = node->prev;
	n_new->next = node;
	node->prev = n_new;
	if(node->prev) {
		node->prev->next = n_new;
	}
}

void llist_insert_after(int data, struct llist_node node)
{
	if(!node) {
		return;
	}
	
	struct llist_node n_new = malloc(sizeof(struct llist_node));
	if(!n_new) {
		printf("ERROR: Memory error");
		exit(1);
	}

	n_new->prev = node;
	n_new->next = node->next;
	node->next = n_new;	
	if(node->next) {
		node->next->prev = n_new;
	}
}

	
