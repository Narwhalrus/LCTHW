#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct MyStack {
    //unsigned int size;
    size_t size;
    int items[MAX_SIZE];
} STACK;

STACK *
Stack_create()
{
    STACK *stack = malloc(sizeof(STACK));
    memset(stack, 0, sizeof(STACK));

    return stack;
}

void
Stack_push(STACK *stack, int item)
{
    if(stack->size == MAX_SIZE)
    {
        fputs("ERROR: stack overflow\n", stderr);
        abort();
    }

    stack->items[stack->size++] = item;
}

int
Stack_pop(STACK *stack)
{
    if(stack->size == 0)
    {
        fputs("ERROR: stack underflow\n", stderr);
        abort();
    }

    return stack->items[--stack->size];
}

void
Stack_destroy(STACK *stack)
{
    if(stack)
        free(stack);
}
