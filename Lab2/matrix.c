#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "matrix.h"

void initMatrix(matrix *A, int rows, int cols) {
	A->rows = rows;
	A->cols = cols;
	A->data = malloc (A->rows*A->cols*sizeof(int));
}



//void sendData(matrix A, int *arr, MPI_Comm world) {
int *sendData(matrix A, MPI_Comm world) {
	int worldSize, rank;

	MPI_Comm_size(world, &worldSize);
	MPI_Comm_rank(world, &rank);

	int *sendcounts = malloc(worldSize*sizeof(int));
	int start = A.rows / worldSize;
	for (int i = 0; i < worldSize; i++) {
		sendcounts[i] = start;
	}
	for (int i = 0; i < A.rows%worldSize; i++) {
		sendcounts[i]++;
	}

	if (rank == 0) {
		for (int i = 0; i < worldSize; i++) {
		//	printf("Will scatter %d rows to rank %d\n", sendcounts[i], i);
		}
	}
	
	for (int i = 0; i < worldSize; i++) {
		//printf("SENDCOUNTS %d and COLS %d\n", sendcounts[i], A.cols);
		sendcounts[i] *= A.cols;
	}

	int *displ = malloc(worldSize*sizeof(int));
	displ[0] = 0;
	for (int i = 1; i < worldSize; i++) {
		displ[i] = displ[i-1] + sendcounts[i-1];
	}

	if (rank == 0) {
		for (int i = 0; i < worldSize; i++) {
			//printf("Will start scattering to rank %d at index %d\n", i, displ[i]);
		}
	}
	int *arr;
	arr = malloc(sendcounts[rank]*sizeof(int));

	MPI_Scatterv(
		A.data,
		sendcounts,
		displ,
		MPI_INT,
		arr,
		sendcounts[rank],
		MPI_INT,
		0,
		world
	);

/*
	for (int i = 0; i < sendcounts[rank]; i++) {
		printf("RANK %d arr[%d]: %d \n", rank, i, arr[i]);
	}
*/
	
	free(sendcounts);
	free(displ);

	return arr;
}
matrix addition(matrix A, matrix B, MPI_Comm world) {

	int worldSize, rank;

	MPI_Comm_size(world, &worldSize);
	MPI_Comm_rank(world, &rank);
	
	int *arr = NULL, *arr2 = NULL;

	arr = sendData(A, world);
	arr2 = sendData(B, world);

	matrix C;
	initMatrix(&C, A.rows, A.cols); 
	
	int *sendcounts = malloc(worldSize*sizeof(int));
	int start = A.rows / worldSize;

	for (int i = 0; i < worldSize; i++) {
		sendcounts[i] = start;
	}
	for (int i = 0; i < A.rows%worldSize; i++) {
		sendcounts[i]++;
	}
	for (int i = 0; i < worldSize; i++) {
		sendcounts[i] *= A.cols;
	}

	int *displ = malloc(worldSize*sizeof(int));
	displ[0] = 0;
	for (int i = 1; i < worldSize; i++) {
		displ[i] = displ[i-1] + sendcounts[i-1];
	}
	int *results = malloc(sendcounts[rank]*sizeof(int));	

	for (int i = 0; i < sendcounts[rank]; i++) {
		results[i] = arr[i] + arr2[i];
	}

	MPI_Gatherv(
		results,
		sendcounts[rank],
		MPI_INT,
		C.data,
		sendcounts,
		displ,
		MPI_INT,
		0,		
		world	
	);

	free(sendcounts);
	free(displ);
	
	free (results);

	free(arr);
	free(arr2);
	return C;
}

matrix subtraction(matrix A, matrix B, MPI_Comm world) {

	int worldSize, rank;

	MPI_Comm_size(world, &worldSize);
	MPI_Comm_rank(world, &rank);
	
	int *arr = NULL, *arr2 = NULL;

	arr = sendData(A, world);
	arr2 = sendData(B, world);

	matrix C;
	initMatrix(&C, A.rows, A.cols); 
	
	int *sendcounts = malloc(worldSize*sizeof(int));
	int start = A.rows / worldSize;

	for (int i = 0; i < worldSize; i++) {
		sendcounts[i] = start;
	}
	for (int i = 0; i < A.rows%worldSize; i++) {
		sendcounts[i]++;
	}
	for (int i = 0; i < worldSize; i++) {
		sendcounts[i] *= A.cols;
	}

	int *displ = malloc(worldSize*sizeof(int));
	displ[0] = 0;
	for (int i = 1; i < worldSize; i++) {
		displ[i] = displ[i-1] + sendcounts[i-1];
	}
	int *results = malloc(sendcounts[rank]*sizeof(int));	

	for (int i = 0; i < sendcounts[rank]; i++) {
		results[i] = arr[i] - arr2[i];
	}

	MPI_Gatherv(
		results,
		sendcounts[rank],
		MPI_INT,
		C.data,
		sendcounts,
		displ,
		MPI_INT,
		0,		
		world	
	);

	free(sendcounts);
	free(displ);
	
	free (results);

	free(arr);
	free(arr2);
	return C;
}

matrix transpose(matrix A) {
//	ACESSS(

	matrix T;
	initMatrix(&T, A.cols, A.rows);
	
	for (int i = 0; i < A.rows; i++) {
		for (int j = 0; j < A.cols; j++) {
			//ACCESS(T, j, i) = ACESS(A, i, j);		
			//printf("ele = %d\n", A.data[A.cols * i + j]);
			T.data[A.rows*j+i] = A.data[A.cols * i + j];
			//printf("teleerlweqr = %d\n", A.data[A.cols * i + j]);
			//ACESS(T, j, i) = 0;
		}
	}	
	//ACCESS(A, i, j) 
	//
	return T;
}

matrix multiplication(matrix A, matrix B) {
	int worldSize, rank;

	MPI_Comm_size(world, &worldSize);
	MPI_Comm_rank(world, &rank);
	
	//int *arr = NULL, *arr2 = NULL;

	//arr = sendData(A, world);
	//arr2 = sendData(B, world);

	matrix C;
	initMatrix(&C, A.rows, B.cols); 
	
	matrix BT;

	if (rank == 0) {
		BT = transpose(B);
	}

	MPI_Bcast(
		BT,	
		BT.rows*BT.cols,
		MPI_INT,
		0,
		world
	);
	

	int *sendcounts = malloc(worldSize*sizeof(int));
	int start = A.rows / worldSize;

	for (int i = 0; i < worldSize; i++) {
		sendcounts[i] = start;
	}
	for (int i = 0; i < A.rows%worldSize; i++) {
		sendcounts[i]++;
	}
	for (int i = 0; i < worldSize; i++) {
		sendcounts[i] *= A.cols;
	}

	int *displ = malloc(worldSize*sizeof(int));
	displ[0] = 0;
	for (int i = 1; i < worldSize; i++) {
		displ[i] = displ[i-1] + sendcounts[i-1];
	}

	int arr* = NULL;
	arr = sendData(A, world);

	int amount = 0;
	while (amount < sendcounts[rank] / start) {
		for (int i = 0; i < ; i++) {
			for (int j = 0; j < A.cols; j++) {

			}
	}

	//int *results = malloc(sendcounts[rank]*sizeof(int));	

	//for (int i = 0; i < sendcounts[rank]; i++) {
	//	results[i] = arr[i] - arr2[i];
	//}

//	for (int i = 0; i < sendcounts[rank]; i++) {
//		[i] = arr[i] - arr2[i];
//	}
	

	
//	T.data[A.rows*j+i] = A.data[A.cols * i + j];

	/*
	MPI_Gatherv(
		results,
		sendcounts[rank],
		MPI_INT,
		C.data,
		sendcounts,
		displ,
		MPI_INT,
		0,		
		world	
	);
	*/

	free(sendcounts);
	free(displ);
	
	//free (results);

	//free(arr);
	//free(arr2);
	free(BT.data);
	return C;

}
