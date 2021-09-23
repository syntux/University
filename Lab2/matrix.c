#include <stdio.h>
#include <mpi.h>

int *addition(int row, int col, int *arr1, int *arr2, MPI_Comm world) {

	int worldSize, rank;

	MPI_Comm_size(world, &worldSize);
	MPI_Comm_rank(world, &rank);

	
}

int main() {
	MPI_Init(NULL, NULL);

	int worldSize, rank;
	MPI_Finalize();

}

