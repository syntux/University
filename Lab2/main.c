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


	int n = 13, m = 2, i, j;

	srand(time(0));
	matrix A, B, C;
	if (rank == 0) {
		initMatrix(&A, n, m);
		initMatrix(&B, n, m); // can change
		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++) {
				ACCESS(A,i,j) = rand() % 10;
				ACCESS(B,i,j) = rand() % 10;
				printf("%4d ", ACCESS(A, i, j));
				//printf("%4d ", ACCESS(B, i, j));
			}
			printf("\n");
		}
		printf("A-----\n");
	}
	else {
		A.data = NULL;
		A.rows = n;
		A.cols = m;

		B.data = NULL;
		B.rows = n;
		B.cols = m;

		C.data = NULL;
		C.rows = n;
		C.cols = m;
	}

	//int *arr = NULL;
	//sendData(A, arr, world);
	//
	

/*
	if (rank == 0) {
		C = addition(A, B, world);

		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++) {
				printf("%4d ", ACCESS(C, i, j));
			}
			printf("\n");
		}
		
	}
*/
//	int *arr = NULL, *arr2 = NULL;
//	sendData(A, arr, world);
	//printf("\n\n\n\n--\n");
//	sendData(B, arr2, world);

	//C = addition(A, B, world);
	C = subtraction(A, B, world);
	

	if (rank == 0) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++) {
				printf("%4d ", ACCESS(B, i, j));
			}
			printf("\n");
		}
		printf("B-----\n");
	}
	
	if (rank == 0) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++) {
				printf("%4d ", ACCESS(C, i, j));
			}
			printf("\n");
		}
		printf("C-----\n");
	}

	free(A.data);
	free(B.data);
	free(C.data);
	//free(arr);
	//free(arr2);
	MPI_Finalize();

	return 0;
}

