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
	//printf("RANK %d\n", rank);
	arr = sendData(A, world);
	arr2 = sendData(B, world);
/*
//////////////////


	sendData(A, arr, world);
	sendData(B, arr2, world);

/

//	int worldSize, rank;

//	MPI_Comm_size(world, &worldSize);
//	MPI_Comm_rank(world, &rank);

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

	arr2 = malloc(sendcounts[rank]*sizeof(int));

	MPI_Scatterv(
		B.data,
		sendcounts,
		displ,
		MPI_INT,
		arr2,
		sendcounts[rank],
		MPI_INT,
		0,
		world
	);

/
	for (int i = 0; i < sendcounts[rank]; i++) {
		printf("RANK %d arr[%d]: %d \n", rank, i, arr[i]);
	}
/
	
	printf("RANK %d\n", rank);
	//free(sendcounts);
	//free(displ);
/////////////////////
*/
	//printf("arr[0] %d arr2[0] %d\n", arr[0], arr2[0]);

	
	matrix C;
	initMatrix(&C, A.rows, A.cols); 
	
/*
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
		printf("rank %d se: %d\n", rank, sendcounts[i]);
	}

	int *displ = malloc(worldSize*sizeof(int));
	displ[0] = 0;
	for (int i = 1; i < worldSize; i++) {
		displ[i] = displ[i-1] + sendcounts[i-1];
	}
*/
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
	//printf("sendcount[0] = %d\n", sendcounts[0]);
	for (int i = 1; i < worldSize; i++) {
		//printf("add rank %d sendcounts %d\n", rank, sendcounts[i]);
		displ[i] = displ[i-1] + sendcounts[i-1];
		//printf("sendcount[%d] = %d\n", i, sendcounts[i]);
	}
	int *results = malloc(sendcounts[rank]*sizeof(int));	

	//printf("arr %d\n", arr[1]);
	for (int i = 0; i < sendcounts[rank]; i++) {
		results[i] = arr[i] + arr2[i];
		//printf("result: %d\n", results[i]);
	}

	//printf("rank: %d\n", rank);
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

	free(arr);
	free(arr2);
	return C;
}
