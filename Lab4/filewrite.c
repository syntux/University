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


	int n = 5, m = 5, i, j;

	MPI_File fh;
	const char *fname = "matrix.data";

	MPI_File_open(
		world,
		fname,
		MPI_MODE_CREATE | MPI_MODE_RDWR,
		MPI_INFO_NULL,
		&fh
	 );
	
	srand(time(0));
	matrix A;
	if (rank == 0) {
		initMatrix(&A, n, m);
		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++) {
				ACCESS(A,i,j) = rand() % 10 + 1;
				printf("%0.1f ", ACCESS(A, i, j));
			}
			printf("\n");
		}

		MPI_File_write(
			fh,
			A.data,
			A.rows*A.cols,
			MPI_DOUBLE,
			MPI_STATUS_IGNORE
		);
	}
	
	else {
		A.data = NULL;
		A.rows = n;
		A.cols = m;
	}

	MPI_Datatype rows;	
	MPI_Type_contiguous(m, MPI_DOUBLE, &rows);
	MPI_Type_commit(&rows);


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
	MPI_File_set_view(
		fh,
		displ[rank] *sizeof(double),//df
		MPI_DOUBLE,
		rows,
		"native",
		MPI_INFO_NULL
	);

//	MPI_File_seek(fh, 0, MPI_SEEK_SET);
	
	//printf("rank %d sendcounts %d and displ %d\n", rank, sendcounts[rank], displ[rank]);
	
	double *a = malloc(sendcounts[rank]*sizeof(double));
	//double *a = calloc(0, sendcounts[rank]*sizeof(double));
	//printf("rank %d amount %lu\n", rank, sendcounts[rank]*sizeof(double));

	MPI_File_read(fh, a, sendcounts[rank]/ A.cols, rows, MPI_STATUS_IGNORE);
//	MPI_File_read_at(fh, displ[rank], a, sendcounts[rank], MPI_DOUBLE, MPI_STATUS_IGNORE);
	//MPI_File_read_at(fh, displ[rank], a, sendcounts[rank], rows, MPI_STATUS_IGNORE);
	//MPI_File_read_at(fh, 0, a, sendcounts[rank], rows, MPI_STATUS_IGNORE);
	//if (rank == 1) MPI_File_read_at(fh, 0, a, sendcounts[rank], rows, MPI_STATUS_IGNORE);
	MPI_Barrier(world);
	
//	if (rank == 1) {
//	printf("A.cols %d sendcounts[rank] / A.cols %d\n", A.cols, sendcounts[rank] / A.cols); 
//	MPI_File_seek(fh, 15, MPI_SEEK_SET);
//	MPI_File_read(fh, a, sendcounts[rank] / A.cols, rows, MPI_STATUS_IGNORE);
//	}

	printf("rank %d\n", rank);
	for (int i = 0; i < sendcounts[rank]; i++) {
		printf("%0.1f ", a[i]);
	}
	printf("\n");
	
	//matrix AC; 
	if (rank != 0) {
		//AC.data = NULL;
		//AC.rows = A.rows;
		//AC.cols = A.cols;
	}
	else {
	//	AC = copyMatrix(A);
	}

	free(A.data);
	//free(AC.data);
	free(sendcounts);
	free(displ);
	MPI_Type_free(&rows);
//	MPI_File_close(&fh);
	free(a);
	MPI_Finalize();

	return 0;
}

