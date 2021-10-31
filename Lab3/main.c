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


	int n = 3, m = 3, i, j;

	srand(time(0));
	matrix A, B, C;
	if (rank == 0) {
		initMatrix(&A, n, m);
		initMatrix(&B, n, 1); // can change
		//memset(A.data, 0, n*m*sizeof(int));
		for (i = 0; i < n; i++) {
			ACCESS(B, i, 0) = i+1;
			for (j = 0; j < m; j++) {
				ACCESS(A,i,j) = rand() % 10 + 1;
				//ACCESS(B,i,j) = rand() % 10 + 1;

				//ACCESS(A,i,j) = 1;
				//ACCESS(B,i,j) = 1;
				printf("%0.1f ", ACCESS(A, i, j));
				//printf("%4d ", ACCESS(B, i, j));
			}
			printf("\n");
		}
		printf("^A-----\n");
	}
	else {
		A.data = NULL;
		A.rows = n;
		A.cols = m;

	//	B.data = NULL;
	//	B.rows = n;
	//	B.cols = m;

		C.data = NULL;
		C.rows = n;
		C.cols = m;
	}

	if (rank == 0) {
		for (i = 0; i < n; i++) {
			//for (j = 0; j < m; j++) {
				printf("%0.1f ", ACCESS(B, i, 0));
			//}
			printf("\n");
		}
		printf("^B-----\n");
	}
	else {
		B.data = NULL;
		B.rows = n;
		B.cols = 1;
	}
	
	
	matrix AC; 
//	if (rank == 0) {
	if (rank != 0) {
		C.data = NULL;
		C.rows = A.rows;
		C.cols = B.cols;	

		AC.data = NULL;
		AC.rows = A.rows;
		AC.cols = A.cols;
	}
	else {
		AC = copyMatrix(A);
	}
	matrix x;
	x = gjElim(AC, B, world);
//	}
	//C = multiplication(A, B, world); 
	C = multiplication(A, x, world); 

	if (rank == 0) {
			for (j = 0; j < C.rows; j++) {
				printf("%0.1f ", ACCESS(C, j, 0));
			}
			printf("\n");
		printf("C MULTI ANSWER-----\n");
	}

	free(A.data);
	free(B.data);
	free(AC.data);
	free(C.data);
	free(x.data);
	//free(arr);
	//free(arr2);
	MPI_Finalize();

	return 0;
}

