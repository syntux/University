all: hello, prime

hello: hello.c
	mpicc -std=c99 hello.c -o hello


prime: prime.c
	mpicc -std=c99 prime.c -lm -o prime

clean:
	rm hello prime

