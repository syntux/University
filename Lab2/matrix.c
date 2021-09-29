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
	if (rank == 0) {
		initMatrix(&C, A.rows, A.cols); 
	}
	else {
		C.data = NULL;
		C.rows = A.rows;
		C.cols = A.cols;
	}
	
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

	matrix T;

	if (!A.data) {
		T.data = NULL;
		T.rows = A.cols;
		T.cols = A.rows;
		return T;
	}

	else {
		initMatrix(&T, A.cols, A.rows);
	}
	

	for (int i = 0; i < A.rows; i++) {
		for (int j = 0; j < A.cols; j++) {
			ACCESS(T, j, i) = ACCESS(A, i, j);
		}
	}	
	return T;
}
matrix multiplication(matrix A, matrix B, MPI_Comm world) {
	int worldSize, rank;

	MPI_Comm_size(world, &worldSize);
	MPI_Comm_rank(world, &rank);
	
	matrix BT;

	BT = transpose(B);
	//initMatrix(&BT, BT.cols, BT.rows);	
	
	if (rank != 0) {
		BT.data = malloc(B.rows*B.cols*sizeof(int));
	//	initMatrix(&BT, A.cols, A.rows);	
	}

	MPI_Bcast(
		BT.data,	
		BT.rows*BT.cols,
		MPI_INT,
		0,
		world
	);
	
//////////////////////////////////////
	
	int *arr= NULL;

	arr = sendData(A, world);

	matrix C;
	initMatrix(&C, A.rows, B.cols); 
	
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

/////////////////////////////////////

	int getRows = sendcounts[rank] / A.cols;
	//printf("GETROWS: %d - start %d - sendcounts[rank] %d\n",getRows, start, sendcounts[rank]);

	int count = 0;

	
	int *newSend; 
	newSend = malloc(worldSize*sizeof(int));
	
	//if (rank == 0) {
	for (int i = 0; i < worldSize; i++) {
		newSend[i] = sendcounts[i] / A.cols * B.cols; 
		//printf("newsend[i] %d\n", newSend[i]);
	}
	//}
	int *arr2;
	arr2 = malloc(newSend[rank]*sizeof(int));
	if (rank == 0) {
		for (int i = 0; i < getRows*B.cols; i++) {
		//printf("arr2[%d] %d\n", i, arr[i]);
	}
	}

	int *newDispl = malloc(worldSize*sizeof(int));
	newDispl[0] = 0;
	for (int i = 1; i < worldSize; i++) {
		newDispl[i] = newDispl[i-1] + newSend[i-1];
	}

	int sum;
	for (int i = 0; i < getRows; i++) {
		for (int j = 0; j < BT.rows; j++) {
			sum = 0;
			for (int k = 0; k < A.cols; k++) {
				sum += arr[(i*A.cols)+k] * ACCESS(BT, j, k);
			}
			arr2[count] = sum;
			count++;
		}
	}

	MPI_Gatherv(
		arr2,
		newSend[rank],
		MPI_INT,
		C.data,
		newSend,
		newDispl,
		MPI_INT,
		0,		
		world	
	);

	free(sendcounts);
	free(displ);
	

	free(newSend);
	free(newDispl);
	//free (results);

	free(arr);
	free(arr2);
	free(BT.data);
	return C;

}

