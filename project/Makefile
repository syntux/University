all: password 

password: password.c
	mpicc -std=c99 password.c -o password -g -lcrypt

clean:
	rm vector 

