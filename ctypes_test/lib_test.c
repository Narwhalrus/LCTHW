#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lib_test.h"
void *create_memory_space(void)
{
	int i;
	int *buffer = NULL;

	// Allocate block of memory to be used in python
	buffer = malloc(sizeof(int) * 200);
	if(!buffer) {
		puts("OH SHIT!!");
		return NULL;
	}	

	printf("C addr: %p\n", buffer);

	for(i = 0; i < 200; i++) {
		buffer[i] = i;
	}

	return (void *)buffer;
}

struct TStruct *create_struct(void)
{
    struct TStruct *res = malloc(sizeof(struct TStruct));
    if(!res) {
        printf("Ahh!\n");
        return NULL;
    }

    res->a = 1;
    res->b = 2;
    res->c = 3.0f;
    res->d = 4.0;

    return res;
}

void *create_struct_v(void)
{
    struct TStruct *res = malloc(sizeof(struct TStruct));
    if(!res) {
        printf("Ahh!\n");
        return NULL;
    }

    res->a = 1;
    res->b = 2;
    res->c = 3.0f;
    res->d = 4.0;

    return (void *)res;

}

struct TStruct2 *create_struct2(void)
{
    struct TStruct2 *res = malloc(sizeof(struct TStruct2));
    if(!res) {
        printf("Ahh!\n");
        return NULL;
    }

    memset(res, 0, sizeof(struct TStruct2)); 
     
    return res;
}

void print_struct(struct TStruct *mem)
{
    printf("a: %d b: %d c: %f d: %f\n", mem->a, mem->b, mem->c, mem->d);
}

void print_struct2(struct TStruct2 *mem)
{
    int i = 0;
    printf("a: %d ", mem->a);
    for(i = 0; i < 10; i++) {
        printf("spares_%d: %d ", i, mem->spares[i]);
    }
    printf("b: %d\n", mem->b);
}

void free_struct(struct TStruct *mem)
{
    if(mem) {
        free(mem);
    }
}
void alter_memory(void *buffer, unsigned int offset) 
{
    *(unsigned char *)(buffer + offset) = 0xFF;
}

void get_memory(void *buffer, unsigned int offset)
{
    printf("%d\n", *(unsigned int *)(buffer + offset));
}

void destroy_memory_space(void *buffer) {
	if(buffer) {
		free(buffer);
	}
}

