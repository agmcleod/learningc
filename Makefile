CFLAGS=-Wall -g -DNDEBUG -std=c99
SRCS=ex1.c ex10.c ex11.c ex12.c ex13.c ex14.c ex15.c ex16-2.c ex16.c ex17.c ex18.c ex3.c ex5.c ex6.c ex7.c ex8.c ex9.c ex23.
PROGS=$(patsubst %.c,%,$(SRCS))

all: dir object.o $(PROGS) ex19 ex22

dir:
	mkdir -p bin

%: %.c
	$(CC) $(CFLAGS) -o bin/$@ $<

object.o:
	$(CC) $(CFLAGS) -O -c object.c -o bin/object.o

ex19: object.o
	$(CC) $(CFLAGS) -L bin -l object.o ex19.c -o bin/ex19

ex22:
	$(CC) $(CFLAGS) -c -o ex22.o ex22.c
	$(CC) $(CFLAGS) ex22_main.c ex22.o -o bin/ex22_main

clean:
	rm -rf bin/*