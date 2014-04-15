CFLAGS=-Wall -g -DNDEBUG -std=c99

all: ex1 ex3 ex5 ex6 ex7 ex8 ex9 e10 ex11 ex12 ex13 ex14 ex15 ex16 ex17 ex18 ex19 ex20

ex19: object.o

clean:
	rm -f ex1 ex3 ex5 ex6 ex7 ex8 ex9 ex10 ex11 ex12 ex13 ex14 ex15 ex16 ex17 ex18 ex19 object.o ex20