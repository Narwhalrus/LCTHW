#include <stdio.h>
#include "MyStack.h"

int main()
{
    int i;
    STACK *ss = Stack_create();

    printf("init size = %d\n", ss->size);

    for(i = 0; i < MAX_SIZE; i++) {
        Stack_push(ss, i);
        printf("%05d\n", i);
    }

    for(i = 0; i < MAX_SIZE; i++)
        printf("%05d\n", Stack_pop(ss));

    return 0;
}


