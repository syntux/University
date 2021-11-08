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
				//printf("%4d ", ACCESS(B, i, j));
			}
		}
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
	if (rank == 0) {
	printMatrix(A);
	printf("^A-----\n");
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
	matrix CA;

	if (rank == 0) {
		CA = transpose(A);
		for (i = 0; i < A.cols; i++) {
			for (j = 0; j < A.rows; j++) {
				//printf("%4d ", ACCESS(CT, i, j));
				
			}
		}
	}
	else {
		CA.data = NULL;
		CA.rows = A.cols;
		CA.cols = A.rows;
	}


	int	multiRows = 3;
	int	multiCols = 5;
	if (rank == 0) {
		free(C.data);
		free(B.data);

		initMatrix(&B, multiRows, multiCols);
		for (i = 0; i < multiRows; i++) {
			for (j = 0; j < multiCols; j++) {
				//ACCESS(B,i,j) = rand() % 10 + 1;
				ACCESS(B,i,j) = 1;
			}
		}
		printMatrix(B);
		printf("NEW B-----\n");

		//initMatrix(&C, 4, 4);
	}
	else {
		B.data = NULL;
		B.rows = multiRows;
		B.cols = multiCols;
		C.data = NULL;
		C.rows = A.rows;
		C.cols = B.cols;
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
	C = multiplicationType(A, B, world); 

	/*
	if (rank == 0) {
		for (i = 0; i < C.rows; i++) {
			for (j = 0; j < C.cols; j++) {
				printf("%4d ", ACCESS(C, i, j));
			}
			printf("\n");
		}
		printf("C MULTI ANSWER-----\n");
	}
	*/
	if (rank == 0) {
printMatrix(C);
	printf("C MULTI ANSWER-----\n");
}

	n = 4000;
	m = 4100;

	multiRows = 4000;
	multiCols = 3000;

	free(A.data);
	free(B.data);
	free(C.data);	


	if (rank == 0) {
		 

		initMatrix(&A, n, m);
		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++) {
				ACCESS(A,i,j) = rand() % 100 + 1;
			}
		}
		initMatrix(&B, multiRows, multiCols); 
		for (i = 0; i < multiRows; i++) {
			for (j = 0; j < multiCols; j++) {
				ACCESS(B,i,j) = rand() % 100 + 1;
			}
		}
		//printf("NEW B-----\n");
	}
	else {
		A.data = NULL;
		A.rows = n;
		A.cols = m;

		B.data = NULL;
		B.rows = multiRows;
		B.cols = multiCols;

		C.data = NULL;
		C.rows = A.rows;
		C.cols = B.cols;
	}

	double t1 = MPI_Wtime();	
	//C = multiplicationType(A, B, world); 
	C = multiplication(A, B, world); 
	double t2 = MPI_Wtime();

	if (rank == 0) {	
		printf("The (%d x %d) * (%d x %d) took %f seconds\n", A.rows, A.cols, B.rows, B.cols, t2-t1);
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

