#include <stdio.h>
#include "llist.h"

int filter_pred(const int *data)
{
    return *data % 2;
}

int map_pred(const int *data)
{
    return *data + 2;
}

int main()
{
    int i = 0;

    struct llist_node *begin = llist_create_node(0);
    struct llist *ll = llist_create();
    llist_append(ll, begin);

    struct llist_node *loop_node = ll->begin;
    for(i = 1; i < 20; i++)
    {
        llist_insert_after(ll, loop_node, llist_create_node(i));
        loop_node = loop_node->next;
    }

    printf("Created list...\n"); 
    llist_print(ll);
    printf("size: %d\n\n", ll->size);

/*
    struct llist_node *temp;
    LL_TRAVERSE_FWD(loop_node, ll) {
        if(loop_node->data % 2) {
            temp = loop_node->prev;
            llist_remove(ll, loop_node);
            loop_node = temp;
            while(!loop_node) {
                if(loop_node->data % 2) {
                    temp = loop_node->prev;
                    llist_remove(ll, loop_node);
                    loop_node = temp;
                }
            }
        }
    }
*/

    llist_filter(ll, filter_pred);
    printf("After filtering odd numbers...\n");
    llist_print(ll);
    printf("size: %d\n\n", ll->size);

    llist_map(ll, map_pred);
    printf("After mapping +2 to all elements...\n");
    llist_print(ll);
    printf("size: %d\n", ll->size);

    llist_destroy(ll);

    return 0;
}
