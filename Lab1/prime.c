#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
	// Make sure argument count is 2 to test a number
	if (argc != 2) {
		printf("Need number argument\n");
		return 0;
	}
	
	// Initialize MPI and define world
	MPI_Init(&argc, &argv);
	MPI_Comm world = MPI_COMM_WORLD;

	int worldSize, rank;

	// Needs to be ull to keep up with argument as ull
	unsigned long long i;

	// Get the amount of nodes and node rank
	MPI_Comm_size(world, &worldSize);
	MPI_Comm_rank(world, &rank);
	
	// Prime to see if found (1) or not (0)
	// Count array to keep track of first 3 tested numbers
	// and amount to keep track for array
	int prime = 1, count[3] = {0, 0, 0}, amount = 0;

	// Take string command line argument and turn it ull
	unsigned long long n = strtoull(argv[1], NULL, 10);
	
	// Loop so that the first rank tests 2, (2 + p), (2 + p + p)
	// and second rank tests 3, (3 + p), (3 + p + p) and so on..
	for (i = 2 + rank; i <= sqrt(n); i += worldSize) {
		if (amount != 3) {
			// Get first few tested divsors while incrementing
			count[amount++] = i;
		}

		// Test if argument is divisible by i and if factor not found yet 
		// if factor found change prime to 0 to show the number isn't prime
		if (n % i == 0 && prime != 0) {
			printf("Not prime (0) from node: %d. Factor: %llu\n", rank, i);
			prime = 0;
		}

		// Does not work as intended
		
		//if (n % i == 0) {
		//	printf("Factor found: %d from node %d\n", i, rank);
		//	MPI_Abort(world, error);
		//}
	}

	// After going through loop without finding a factor it is considered 
	// prime for this node. Has to be prime for every node to be prime
	if (prime != 0) {
		printf("Prime (1) from node: %d\n", rank);
	}

	// Print the first 3 numbers tested for each node and rank	
	printf("\nFirst few numbers are %d, %d, %d, ... from node: %d\n\n", count[0], count[1], count[2], rank);
	
	// Finalize MPI
	MPI_Finalize();
	return 0;
}
