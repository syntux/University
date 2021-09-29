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

	initMatrix(&T, A.cols, A.rows);
	

	for (int i = 0; i < A.rows; i++) {
		for (int j = 0; j < A.cols; j++) {
			//ACCESS(T, j, i) = ACESS(A, i, j);		
			//printf("ele = %d\n", A.data[A.cols * i + j]);
			//T.data[A.rows*j+i] = A.data[A.cols * i + j];
			ACCESS(T, j, i) = ACCESS(A, i, j);
			//printf("T- %d\nA- %d\n", T.data[A.rows*j+i], A.data[A.cols *i + j]);
			//printf("teleerlweqr = %d\n", A.data[A.cols * i + j]);
			//ACESS(T, j, i) = 0;
		}
	}	
	//ACCESS(A, i, j) 
	//
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
	
/*
	if (rank == 1) {
	printf("Hi\n");
	for (int i = 0; i < BT.rows; i++) {
		for (int j = 0; j < BT.cols; j++) {
			printf("%4d(%d, %d) ", ACCESS(BT, i, j), i, j);
		}
		printf("MUL----\n");
	}
	}
*/
//////////////////////////////////////
	
	int *arr = NULL, *arr2 = NULL;

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



//	matrix temp;
//	initMatrix(&temp, start, sendcounts[rank] / start);
//	temp.data = arr;
	

	//arr2 = malloc(sendcounts[rank]*sizeof(int));
	//arr2 = calloc(sendcounts[rank],sizeof(int));

	//int getRows = sendcounts[rank] / start;
	int getRows = sendcounts[rank] / A.cols;
	//printf("GETROWS: %d - start %d - sendcounts[rank] %d\n",getRows, start, sendcounts[rank]);

	//int count = displ[rank];
	//
	arr2 = malloc(getRows * A.rows*sizeof(int));
	int count = 0;

	
	//printf("Sendcounts[%d], %d\n", rank, sendcounts[rank]);
	//printf("B ROWS %d\n", B.rows);
	//
	int *newSend; 
	newSend = malloc(worldSize*sizeof(int));
	
	//if (rank == 0) {
	for (int i = 0; i < worldSize; i++) {
		newSend[i] = sendcounts[i] / A.cols * A.rows; 
		//printf("newsend[i] %d\n", newSend[i]);
	}
	//}
	if (rank == 0) {
		for (int i = 0; i < worldSize; i++) {
	//	printf("newsend[i] %d\n", newSend[i]);
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
				printf("COUNT: %d RANK %d k %d\n", count, rank, BT.rows);
				//sum += ACCESS(temp,i,k) * ACCESS(BT, j, k);
				sum += arr[(i*A.cols)+k] * ACCESS(BT, j, k);
				//printf("NUM %d Arr %d\n", i * BT.cols + k, arr[(i*BT.cols+k)]);
//				printf("arr[] %d *  ACCESS %d j - %d k - %d i - %d\n", arr[(i*A.cols)+k], ACCESS(BT, j, k), j, k, i);
//				printf("SUM: %d\n i %d j %d k %d arr %d BT %d\n", sum, i, j, k, arr[(i*A.cols)+k], ACCESS(BT, j, k));
				//printf("i*A.cols+k %d --- rank %d\n", (i*A.cols)+k, rank);
				//printf("j [%d] i[%d] rank - %d\n", j, k, rank);
				//printf("[%d] rank %d\n", ACCESS(BT, j, k), rank);
			}
			//printf("COUNT: %d - rank %d\n", count, rank);
			arr2[count] = sum;
			count++;
			//printf("COUNT: %d\n rank: %d\n", count, rank);
		}
		//ACCESS(C, j, k) = sum;
	}

//	for (int i = 0; i < worldSize; i++) {
//		printf("DISPL %d\n", newDispl[rank]);
//	}
/*
	MPI_Gatherv(
		arr2,
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


	// BREAKS ABOVE


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

