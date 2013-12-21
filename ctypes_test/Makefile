# This makefile is stupid

objs:
	gcc -c -Wall -fpic lib_test.c

all: objs 
	gcc -g -Wall -shared -o libtest.so lib_test.o

clean:
	rm libtest.so lib_test.o

