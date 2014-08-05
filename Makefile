CFLAGS=-Wall -g -DNDEBUG -std=c99
SRCS=$(wildcard *.c)
PROGS=$(patsubst %.c,%,$(SRCS))
# all: dir ex1 ex3 ex5 ex6 ex7 ex8 ex9 ex10 ex11 ex12 ex13 ex14 ex15 ex16 ex17 ex18 ex19

all: dir object.o $(PROGS)

dir:
	mkdir -p bin

%: %.c
	$(CC) $(CFLAGS) -L bin -l object.o -o bin/$@ $<

object.o:
	$(CC) $(CFLAGS) -O -c object.c -o bin/object.o

clean:
	rm -f bin/*