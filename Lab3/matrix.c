#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "matrix.h"

void initMatrix(matrix *A, int rows, int cols) {
	A->rows = rows;
	A->cols = cols;
	A->data = malloc (A->rows*A->cols*sizeof(double));
}

matrix copyMatrix(matrix A) {
	matrix AC;

	initMatrix(&AC, A.rows, A.cols);
	for (int i = 0; i < A.rows; i++) {
		for (int j = 0; j < A.cols; j++) {
			ACCESS(AC, i, j) = ACCESS(A, i, j);
		}
	}

	return AC;
}

//void sendData(matrix A, int *arr, MPI_Comm world) {
double *sendData(matrix A, MPI_Comm world) {
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
			//printf("Will scatter %d rows to rank %d\n", sendcounts[i], i);
		}
	}
	
	for (int i = 0; i < worldSize; i++) {
		//printf("SENDCOUNTS %d and COLS %d\n", sendcounts[i], A.cols);
		sendcounts[i] *= A.cols;
		//printf("sendcounts %d\n", sendcounts[i]);
	}

	int *displ = malloc(worldSize*sizeof(int));
	displ[0] = 0;
	for (int i = 1; i < worldSize; i++) {
		displ[i] = displ[i-1] + sendcounts[i-1];
		//printf("DISPL %d\n", displ[i]);
	}

	if (rank == 0) {
		for (int i = 0; i < worldSize; i++) {
			//printf("Will start scattering to rank %d at index %d\n", i, displ[i]);
		}
	}
	double *arr;
	arr = malloc(sendcounts[rank]*sizeof(double));

	MPI_Scatterv(
		A.data,
		sendcounts,
		displ,
		MPI_DOUBLE,
		arr,
		sendcounts[rank],
		MPI_DOUBLE,
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
	
	double *arr = NULL, *arr2 = NULL;

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
	double *results = malloc(sendcounts[rank]*sizeof(double));	

	for (int i = 0; i < sendcounts[rank]; i++) {
		results[i] = arr[i] + arr2[i];
	}

	MPI_Gatherv(
		results,
		sendcounts[rank],
		MPI_DOUBLE,
		C.data,
		sendcounts,
		displ,
		MPI_DOUBLE,
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

	
	double *arr = NULL, *arr2 = NULL;

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
	double *results = malloc(sendcounts[rank]*sizeof(double));	

	for (int i = 0; i < sendcounts[rank]; i++) {
		results[i] = arr[i] - arr2[i];
	}

	MPI_Gatherv(
		results,
		sendcounts[rank],
		MPI_DOUBLE,
		C.data,
		sendcounts,
		displ,
		MPI_DOUBLE,
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
		BT.data = malloc(B.rows*B.cols*sizeof(double));
	//	initMatrix(&BT, A.cols, A.rows);	
	}

	MPI_Bcast(
		BT.data,	
		BT.rows*BT.cols,
		MPI_DOUBLE,
		0,
		world
	);
	
//////////////////////////////////////
	
	double *arr= NULL;

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
	double *arr2;
	arr2 = malloc(newSend[rank]*sizeof(double));
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

	double sum;
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
		MPI_DOUBLE,
		C.data,
		newSend,
		newDispl,
		MPI_DOUBLE,
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

matrix gjElim(matrix A, matrix B, MPI_Comm world) {
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
	for (int i = 0; i < worldSize; i++) {
		sendcounts[i] *= A.cols;
	}


	int *displ = malloc(worldSize*sizeof(int));
	displ[0] = 0;
	int global_start = 0;
	int global_end = A.rows;
	for (int i = 1; i < worldSize; i++) {
		displ[i] = displ[i-1] + sendcounts[i-1];
		//printf("DISPL %d\n", displ[i]/A.rows);
		if (rank == i) {
			global_start = displ[i]/A.cols;
		}
		else if (rank+1 == i) {
			global_end = displ[i]/A.cols;
		}
	}
	//printf("RANK %d GLOBAL START %d\n", rank, global_start);
	//printf("RANK %d GLOBAL END %d\n", rank, global_end);
	
	int *sendcountsB = malloc(worldSize*sizeof(int));
	int start2 = B.rows / worldSize;

	for (int i = 0; i < worldSize; i++) {
		sendcountsB[i] = start2;
	}
	for (int i = 0; i < B.rows%worldSize; i++) {
		sendcountsB[i]++;
	}
	for (int i = 0; i < worldSize; i++) {
		sendcountsB[i] *= B.cols;
	}


	int *displ2 = malloc(worldSize*sizeof(int));
	displ2[0] = 0;
	for (int i = 1; i < worldSize; i++) {
		displ2[i] = displ2[i-1] + sendcountsB[i-1];
	}

	
	double *arr= NULL;
	arr = sendData(A, world);

	double *arr2= NULL;
	arr2 = sendData(B, world);

	matrix LA;
	LA.data = arr;
	LA.rows = global_end - global_start;
	LA.cols = A.cols;	

	matrix LB;
	LB.data = arr2;
	LB.rows = LA.rows;
	LB.cols = 1;
	
	for (int i = 0; i < LB.rows; i++) {
		//printf("rank %d and %0.1f ", rank, ACCESS(LB, i, 0));
	}


	int localK = 0;
	int root = 0;
	for (int k = 0; k < A.rows; k++) {
		double *pivotRow;
		pivotRow = malloc(A.cols*sizeof(double));
		double *pivotRow2;
		pivotRow2 = malloc(B.cols*sizeof(double));
		if (k >= global_start && k < global_end) {
		//	printf("RANKKKKKKKK %d\n", rank);
			// bcast pivot row
			// use kth element in pivot row as pivot
			for (int i = 0; i < A.cols; i++) {
				pivotRow[i] = ACCESS(LA, localK, i);
				//printf("pivotRow %0.1f\n", pivotRow[i]);
			}	
			for (int i = 0; i < B.cols; i++) {
				pivotRow2[i] = ACCESS(LB, localK, i);
			}
			//for (int a = 0; a < A.cols; a++) {
				// Need to know local and global
			//	vs[a] = ACCESS(LA, 
			//}
			root = rank;
			for (int i = 0; i < worldSize; i++) {
				if (i != rank) {
					MPI_Send(
						&root,
						1,
						MPI_INT,
						i,
						0,
						world
					);
				}
			}
			localK++;
		}
		else {
			MPI_Recv(
			&root,
			1,
			MPI_INT,
			MPI_ANY_SOURCE,
			0,
			world,
			MPI_STATUS_IGNORE		
			);
		}
		MPI_Bcast(
			pivotRow,
			A.cols,
			MPI_DOUBLE,
			root,
			world
		);
		MPI_Bcast(
			pivotRow2,
			B.cols,
			MPI_DOUBLE,
			root,
			world
		);
		MPI_Barrier(world);
		/*
 		printf("\nPivot row rank %d: ", rank);
		for (int i = 0; i < A.cols; i++) {
			printf("%0.1f ", pivotRow[i]);	
		}
		printf("\n");
		*/
		double vs[LA.rows];
		for (int a = 0; a < LA.rows; a++) {
			//vs[a] = ACCESS(LA, a, k) / ACCESS(A, k, k);
			//printf("LA %0.1f ", ACCESS(LA, a, k));
			vs[a] = ACCESS(LA, a, k) / pivotRow[k];
			//printf("%0.1f ", vs[a]);
		}
		//printf("\n");
		/*
		for (int r = 0; r < A.rows; r++) {
			if (r != k) {
				for (int c = 0; c < A.cols; c++) {
					//ACCESS(A, r, c) = ACCESS(A, r, c) - (vs[r] * ACCESS(A, k, c));
				}
				for (int c = 0; c < A.cols; c++) {
					//ACCESS(B, r, c) = ACCESS(B, r, c) - (vs[r] * ACCESS(B, k, c));
				}	
			}
		}
		
		*/
		for (int r = 0; r < LA.rows; r++) {
			if (rank == root) {
				if (r != localK-1) {
					for (int c = 0; c < LA.cols; c++) {
						ACCESS(LA, r, c) = ACCESS(LA, r, c) - (vs[r] * pivotRow[c]);
					}
					//for (int c = 0; c < LB.rows; c++) {
					for (int c = 0; c < LB.cols; c++) {
						//printf("b R: %d and rank %d\n", r, rank);
						//LB.data[c] = LB.data[c] - (vs[r] *  LB.data[c]);
						//LB.data[c] = LB.data[c] - (vs[r] *  pivotRow2[c]);
						LB.data[r] = LB.data[r] - (vs[r] *  pivotRow2[c]);
					}
				}
			}
			else {
				for (int c = 0; c < LA.cols; c++) {
					ACCESS(LA, r, c) = ACCESS(LA, r, c) - (vs[r] * pivotRow[c]);
				}
				//for (int c = 0; c < LB.rows; c++) {
				for (int c = 0; c < LB.cols; c++) {
					//ACCESS(LB, r, c) = ACCESS(LB, r, c) - (vs[r] * ACCESS(LB, r, c));	
					//LB.data[c] = LB.data[c] - (vs[r] *  LB.data[c]);
					//LB.data[c] = LB.data[c] - (vs[r] *  pivotRow2[c]);
					LB.data[r] = LB.data[r] - (vs[r] *  pivotRow2[c]);
				}
			}
		}

		/*	
		if (rank == root) {
			for (int r = 0; r < LA.rows; r++) {
				if (r != localK-1) {
					for (int c = 0; c < LA.cols; c++) {
						ACCESS(LA, r, c) = ACCESS(LA, r, c) - (vs[r] * pivotRow[c]);
					}
					for (int c = 0; c < LB.cols; c++) {
						LB.data[r] = LB.data[r] - (vs[r] * pivotRow2[c]);
					}
				}
			
			}
		}
		else {
			for (int r = 0; r < LA.rows; r++) {
				for (int c = 0; c < LA.cols; c++) {
					ACCESS(LA, r, c) = ACCESS(LA, r, c) - (vs[r] * pivotRow[c]);
				}
				for (int c = 0; c < LB.cols; c++) {
					LB.data[r] = LB.data[r] - (vs[r] * pivotRow2[c]);
				}
			}
		}
		*/
		free(pivotRow);
		free(pivotRow2);
	}// Delete if comments deleted

	/*
	for (int i = 0; i < LA.rows; i++) {
		for (int j = 0; j < LA.cols; j++) {
			printf("%0.1f ", ACCESS(LA, i, j));
		}
		printf("\n");
	}	
	*/
	int count = 0;
	//printf("gs %d and ge %d\n", global_start, global_end);
	for (int i = global_start; i < global_end; i++) {
		//printf("RANK %d and i %d\n", rank, i);
		//printf("rank %d and LA %0.1f\n", rank, ACCESS(LA, count, i));
		ACCESS(LB, count, 0) = ACCESS(LB, count, 0) / ACCESS(LA, count, i);
		printf("RANK %d X %0.5f\n", rank, ACCESS(LB, count, 0));
		ACCESS(LA, count, i) = ACCESS(LA, count, i) / ACCESS(LA, count, i);
		count++;
	}

	for (int i = 0; i < LA.rows; i++) {
		for (int j = 0; j < LA.cols; j++) {
	//		printf("%0.1f ", ACCESS(LA, i, j));
		}
	//	printf("\n");
	}	
	MPI_Barrier(world);
	for (int i = 0; i < LB.rows; i++) {
		//printf("rank %d and %0.2f ", rank, ACCESS(LB, i, 0));
	}
	//printf("\n");
	//
	matrix x;
	if (rank == 0) {
		initMatrix(&x, B.rows, B.cols);
	}
	else {
		x.rows = B.rows;
		x.cols = B.cols;
		x.data = NULL;
	}
	
	MPI_Gatherv(
		LB.data,
		sendcountsB[rank],
		MPI_DOUBLE,
		x.data,
		sendcountsB,
		displ2,
		MPI_DOUBLE,
		0,
		world					
	);
/*
		for (int i = 0; i < A.rows; i++) {
			for (int j = 0; j < A.rows; j++) {
				printf("%0.1f ", ACCESS(A, i, j));
			}
			printf("\n");
		}
		printf("^^^A\n");
		for (int i = 0; i < B.rows; i++) {
				printf("%0.1f ", ACCESS(B, i, 0));
		}
		printf("\n^^^^^B\n");
	}

		printf("\n");
*/
/*
	for (int i = 0; i < A.rows; i++) {
		ACCESS(B, i, 0) = ACCESS(B, i, 0) / ACCESS(A, i, i);
		ACCESS(A, i, i) = ACCESS(A, i, i) / ACCESS(A, i, i);
	}

	for (int i = 0; i < A.rows; i++) {
		for (int j = 0; j < A.rows; j++) {
			printf("%0.1f ", ACCESS(A, i, j));
		}
		printf("\n\n");
	}
	for (int i = 0; i < B.rows; i++) {
				printf("%0.1f ", ACCESS(B, i, 0));
		}
		printf("\n^^^^^B\n");
*/
	//free(arr);

	free(LA.data);
	free(LB.data);
	free(sendcounts);
	free(sendcountsB);
	free(displ);
	free(displ2);

	return x;
}
