#include <stdio.h>
#include <stdlib.h>

#include "llist.h"

/**
 * Implements basic list functionality
 */
struct llist_node *llist_create_node(int data)
{
    struct llist_node *node = malloc(sizeof(struct llist_node));
	if(!node) {
		printf("ERROR: Memory error");
		exit(1);
	}

    node->data = data;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

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

void llist_destroy(struct llist *ll)
{
    if(ll)
    {
        struct llist_node *loop_node;
        if(ll->end) {
            for(loop_node = ll->end->prev; loop_node; loop_node = loop_node->prev)
            {
                free(loop_node->next); 
            }
            free(ll->begin);
        }
        free(ll);
    }
}

void llist_insert_before(struct llist *ll, struct llist_node *node, struct llist_node *new_node)
{
	if(!node) {
		return;
	}

	new_node->prev = node->prev;
	new_node->next = node;
	if(node->prev) {
		node->prev->next = new_node;
	} else {
        ll->begin = new_node;
    }
	node->prev = new_node;
    ll->size++;
}

void llist_insert_after(struct llist *ll, struct llist_node *node, struct llist_node *new_node)
{
	if(!node) {
		return;
	}

	new_node->prev = node;
	new_node->next = node->next;
	if(node->next) {
		node->next->prev = new_node;
	} else {
        ll->end = new_node;
    }
    node->next = new_node;
    ll->size++;
}

void llist_append(struct llist *ll, struct llist_node *new_node)
{
    ll->size++;
    if(!ll->end) {
        ll->end = new_node;
        ll->begin = new_node;
        return;
    }
    new_node->prev = ll->end;
    ll->end->next = new_node;
    ll->end = new_node;
}

void llist_prepend(struct llist *ll, struct llist_node *new_node)
{
    ll->size++;
    if(!ll->begin) {
        ll->begin = new_node;
        ll->end = new_node;
        return;
    }
    new_node->next = ll->begin;
    ll->begin->prev = new_node;
    ll->begin = new_node;
}

void llist_remove(struct llist *ll, struct llist_node *node)
{
    if(!node) {
        return;
    }
    
    if(node->prev) {
        node->prev->next = node->next;
    } else {
        ll->begin = node->next;
    }

    if(node->next) {
        node->next->prev = node->prev;
    } else {
        ll->end = node->prev;
    }

    free(node);
    ll->size--;
}

void llist_print(struct llist *ll)
{
    struct llist_node *loop_node;
    LL_TRAVERSE_FWD(loop_node, ll) {
        printf("%d ", loop_node->data);
    }
    printf("\n");
}

/**
 * Things...
 */
void llist_map(struct llist *ll, map_cb map)
{
    struct llist_node *loop_node;
    LL_TRAVERSE_FWD(loop_node, ll) {
        loop_node->data = map(&loop_node->data);
    }
}

void llist_filter(struct llist *ll, _filter_pred filter)
{
    struct llist_node *loop_node, *temp;
    loop_node = ll->begin;
    while(loop_node) {
        temp = loop_node->next;
        if(filter(&loop_node->data)) {
            llist_remove(ll, loop_node);
        }
        loop_node = temp;
    }     
}
