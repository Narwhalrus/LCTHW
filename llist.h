#ifndef _LLIST_H_
#define _LLIST_H_

#include <stdarg.h>

struct llist_node {
    int data;
    struct llist_node *prev;
    struct llist_node *next;
};

struct llist {
    struct llist_node *begin;
    struct llist_node *end;
    size_t size;
};

/**
 * node: ptr to llist_node
 * list: ptr to llist to traverse
 */
#define LL_TRAVERSE_FWD(node, list) for(node = list->begin; node; node = node->next)
#define LL_TRAVERSE_BKWD(node, list) for(node = list->end; node; node = node->prev)

typedef int (*map_cb)(const int *data);
typedef int (*_filter_pred)(const int *data);

/**
 * Implemented in llist.c
 */
struct llist_node *llist_create_node(int data);
struct llist *llist_create();
struct llist *llist_from_array(size_t size, int *arr);
int *llist_to_array(struct llist *ll);
void llist_destroy(struct llist *ll);
void llist_insert_before(struct llist *ll, struct llist_node *node, struct llist_node *new_node);
void llist_insert_after(struct llist *ll, struct llist_node *node, struct llist_node *new_node);
void llist_append(struct llist *ll, struct llist_node *new_node);
void llist_prepend(struct llist *ll, struct llist_node *new_node);
void llist_remove(struct llist *ll, struct llist_node *node);
void llist_print(struct llist *ll);

void llist_map(struct llist *ll, map_cb map);
void llist_filter(struct llist *ll, _filter_pred filter);
struct llist *llist_concat(size_t num, ...);

struct llist *nsqsort(struct llist *ll);
#endif /* _LLIST_H_ */
