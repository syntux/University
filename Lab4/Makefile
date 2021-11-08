all: main

main: matrix.o main.o matrix.h
	mpicc -std=c99 matrix.o main.o -o main -g -lm

main.o: main.c matrix.h
	mpicc -std=c99 -c main.c -g 

matrix.o: matrix.c matrix.h
	mpicc -std=c99 -c matrix.c 

timing: matrix.o matrix.h timing.o
	mpicc -std=c99 matrix.o timing.o -o timing -lm

timing.o: matrix.o matrix.h 
	mpicc -std=c99 -c timing.c

filewrite: matrix.o matrix.h filewrite.o
	mpicc -std=c99 matrix.o filewrite.o -o filewrite -lm

filewrite.o: matrix.o matrix.h 
	mpicc -std=c99 -c filewrite.c

multi: matrix.o matrix.h multi.o
	mpicc -std=c99 matrix.o multi.o -o multi -lm

multi.o: matrix.o matrix.h 
	mpicc -std=c99 -c multi.c

clean:
	rm -f main.o matrix.o main timing filewrite multi

