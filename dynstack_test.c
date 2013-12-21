#include <stdio.h>
#include <time.h>
#include "MyStack.h"
#include "DynStack.h"

double dyn_time(size_t num_iter)
{
    long start, stop; 
    start = clock();
    printf("%d\n", start);
    
    int i;
    DSTACK *ss = DStack_create();

//    printf("init size = %d\n", ss->size);

    for(i = 0; i < num_iter; i++) {
        DStack_push(ss, i);
//        printf("%05d\n", i);
    }

    for(i = 0; i < num_iter; i++)
        DStack_pop(ss);
//        printf("%05d\n", DStack_pop(ss));

    stop = clock();
    DStack_destroy(ss);
    printf("%d\n", stop);
    printf("%d\n", stop - start);
    return (double)(stop - start)/CLOCKS_PER_SEC;
}

double static_time(size_t num_iter)
{
    long start = clock();

    int i;
    STACK *ss = Stack_create();

//    printf("init size = %d\n", ss->size);

    for(i = 0; i < num_iter; i++) {
        Stack_push(ss, i);
//        printf("%05d\n", i);
    }

    for(i = 0; i < num_iter; i++)
        Stack_pop(ss);
//        printf("%05d\n", DStack_pop(ss));


    printf("%d\n", clock() - start);
    return (clock() - start)/(double)CLOCKS_PER_SEC;
}

int main()
{
    int i;
    for(i = 0; i < 100; i++)
    {
        printf("DynStack dur: %f -- ", dyn_time(i));
    }

    return 0; 
}


