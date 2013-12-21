#include <stdio.h>
#include <stdlib.h>

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
	int *arr = malloc(sizeof(int) * 10);
	for(i = 0; i < 10; i++) {
		arr[i] = i*3;
	}

    struct llist_node *begin = llist_create_node(0);
    
	struct llist *arrll = llist_from_array(10, arr);
	struct llist *ll = llist_create();
	struct llist *ll_2 = llist_create();
	struct llist *ll_concat;
    llist_append(ll, begin);
	llist_append(ll_2, llist_create_node(0));

    struct llist_node *loop_node = ll->begin;
    for(i = 1; i < 20; i++)
    {
        llist_insert_after(ll, loop_node, llist_create_node(i));
        loop_node = loop_node->next;
    }
	
	loop_node = ll_2->begin;
    for(i = 1; i < 20; i++)
    {
        llist_insert_after(ll_2, loop_node, llist_create_node(i));
        loop_node = loop_node->next;
    }
    printf("Created list from array...\n"); 
    llist_print(arrll);
    printf("size: %d\n\n", ll->size);
    
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
    printf("size: %d\n\n", ll->size);

	printf("Concat test..\n");
	printf("ll: ");
	llist_print(ll);
	printf("ll_2: ");
	llist_print(ll_2);	
	ll_concat = llist_concat(2, ll, ll_2);	
	printf("ll_concat: ");
	llist_print(ll_concat);
    printf("size: %d\n\n", ll_concat->size);

	int *res_arr;
	printf("Creating array from list...\n");
	res_arr = llist_to_array(ll_concat);
	for(i = 0; i < ll_concat->size; i++) {
		printf("%d ", res_arr[i]);
	}
	printf("\n");

	free(res_arr);
	free(arr);
    llist_destroy(ll);
	llist_destroy(ll_2);
	llist_destroy(ll_concat);

    return 0;
}
