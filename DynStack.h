#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct DynStack {
    //unsigned int size;
    size_t capacity;
    size_t size;
    int *items;
} DSTACK;

void
_DStack_alloc_block(DSTACK *stack)
{
    int *items = malloc(sizeof(int) * stack->capacity * 2);
    
    if(items == NULL)
    {
        fputs("ERROR: memory error\n", stderr);
        abort();
    }

    memcpy(items, stack->items, sizeof(int) * stack->capacity);
    free(stack->items);
    
    stack->capacity *= 2;
    stack->items = items;
}

DSTACK *
DStack_create()
{
    
    DSTACK *stack = malloc(sizeof(DSTACK));
    memset(stack, 0, sizeof(DSTACK));

    stack->capacity = 1;
    stack->items = malloc(sizeof(int) * stack->capacity);

    return stack;
}

void
DStack_push(DSTACK *stack, int item)
{
    if(stack->size == stack->capacity)
    {
        _DStack_alloc_block(stack);
    }

    stack->items[stack->size++] = item;
}

int
DStack_pop(DSTACK *stack)
{
    if(stack->size == 0)
    {
        fputs("ERROR: stack underflow\n", stderr);
        abort();
    }

    return stack->items[--stack->size];
}

void
DStack_destroy(DSTACK *stack)
{
    if(stack)
        free(stack->items);
    free(stack);
}
