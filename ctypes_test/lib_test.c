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

void destroy_memory_space(void *buffer) {
	if(buffer) {
		free(buffer);
	}
}
