CFLAGS = -Wall -g

llist:
	gcc $(CFLAGS) -o llist_test llist_test.c llist.c

all:
	cc $(CFLAGS) -o ex1 ex1.c
	cc $(CFLAGS) -o ex3 ex3.c

clean:
	rm -f llist_test
	rm -f ex1
	rm -f ex3
