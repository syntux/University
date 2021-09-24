#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define INDEX(i, j, n, m) i*m + j
#define ACCESS(A, i, j) A.data[INDEX(i, j, A.rows, A.cols)]
//#define PACCESS(A, i, j) A->data[INDEX(i, j, A->rows, A->cols)]

typedef struct matrix {
	int rows;
	int cols;
	int *data;
} matrix;

void initMatrix(matrix *A, int rows, int cols); 


//void sendData(matrix A, int *arr, MPI_Comm world); 
int *sendData(matrix A, MPI_Comm world); 

matrix addition(matrix A, matrix B, MPI_Comm world); 

matrix subtraction(matrix A, matrix B, MPI_Comm world);

matrix transpose(matrix A); 

matrix multiplication(matrix A, matrix B) {
#endif
