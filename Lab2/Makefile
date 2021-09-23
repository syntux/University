all: vector main

vector: vector.c
	mpicc -std=c99 vector.c -o vector -g

main: matrix.o main.o matrix.h
	mpicc -std=c99 matrix.o main.o -o main

main.o: main.c matrix.h
	gcc -std=c99 -c main.c

matrix.o: matrix.c matrix.h
	gcc -std=c99 -c matrix.c

clean:
	rm vector 

