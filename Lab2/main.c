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


	int n = 4, m = 3, i, j;

	srand(time(0));
	matrix A, B, C;
	if (rank == 0) {
		initMatrix(&A, n, m);
		initMatrix(&B, n, m); // can change
		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++) {
				//ACCESS(A,i,j) = rand() % 10 + 1;
				//ACCESS(B,i,j) = rand() % 10 + 1;

				ACCESS(A,i,j) = 1;
				ACCESS(B,i,j) = 1;
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
	matrix CA;

	if (rank == 0) {
		CA = transpose(A);
		for (i = 0; i < A.cols; i++) {
			for (j = 0; j < A.rows; j++) {
				//printf("%4d ", ACCESS(CT, i, j));
				printf("%4d ", ACCESS(CA, i, j));
				
			}
			printf("\n");
		}
		printf("AT-----\n\n\n");
	}
	else {
		CA.data = NULL;
		CA.rows = A.cols;
		CA.cols = A.rows;
	}


	if (rank == 0) {
		free(C.data);
		free(B.data);

		initMatrix(&B, 3, 4);
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 4; j++) {
				//ACCESS(B,i,j) = rand() % 10 + 1;
				ACCESS(B,i,j) = 1;
				printf("%4d ", ACCESS(B, i, j));
			}
			printf("\n");
		}
		printf("NEW B-----\n");

		//initMatrix(&C, 4, 4);
	}
	else {
		B.data = NULL;
		B.rows = 3;
		B.cols = 4;
		C.data = NULL;
		C.rows = 4;
		C.cols = 4;
	}
	
	/*if (rank == 0) {
		initMatrix(&C, 3, 4);
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 4; j++) {
				ACCESS(C,i,j) = rand() % 10 + 1;
				printf("%4d ", ACCESS(B, i, j));
			}
			printf("\n");
		}
	}
*/
	C = multiplication(A, B, world); 

	if (rank == 0) {
		printf("IN MIAN?\n");
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				printf("%4d ", ACCESS(C, i, j));
			}
			printf("\n");
		}
		printf("C MULTI-----\n");
	}




	free(A.data);
	free(B.data);
	free(C.data);
	//free(arr);
	//free(arr2);
	free(CA.data);
	MPI_Finalize();

	return 0;
}

