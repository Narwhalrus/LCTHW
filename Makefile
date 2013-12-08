CFLAGS = -Wall -g

all:
	cc $(CFLAGS) -o ex1 ex1.c
	cc $(CFLAGS) -o ex3 ex3.c

clean:
	rm -f ex1
	rm -f ex3
