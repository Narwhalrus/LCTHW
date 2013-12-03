CFLAGS = -Wall -g

all:
	cc $(CFLAGS) -o ex1 ex1.c

clean:
	rm -f ex1
	rm -f ex3
