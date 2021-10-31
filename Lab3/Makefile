all: main

main: matrix.o main.o matrix.h
	mpicc -std=c99 matrix.o main.o -o main

main.o: main.c matrix.h
	mpicc -std=c99 -c main.c

matrix.o: matrix.c matrix.h
	mpicc -std=c99 -c matrix.c

timing: matrix.o matrix.h timing.o
	mpicc -std=c99 matrix.o timing.o -o timing

timing.o: matrix.o matrix.h 
	mpicc -std=c99 -c timing.c

clean:
	rm -f main.o matrix.o main timing

