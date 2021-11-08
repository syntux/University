#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "matrix.h"

int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);

	int rank, worldSize;

	MPI_Comm world = MPI_COMM_WORLD;

	MPI_Comm_size(world, &worldSize);
	MPI_Comm_rank(world, &rank);


	int n = 5, m = 5;

	/*
	srand(time(0));
	matrix A, C;
	if (rank == 0) {
		initMatrix(&A, n, m);
		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++) {
				ACCESS(A,i,j) = rand() % 10 + 1;
				printf("%0.1f ", ACCESS(A, i, j));
			}
			printf("\n");
		}
		printf("^A-----\n");
	}
	else {
		A.data = NULL;
		A.rows = n;
		A.cols = m;

		C.data = NULL;
		C.rows = n;
		C.cols = 1;
	}
	*/

	matrix A;
	A.rows = n;
	A.cols = m;
	powerMethod(A, 7, "matrix.data", world);
	//free(A.data);
	//free(C.data);
	MPI_Finalize();
	free(A.data);

	return 0;
}

