#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
	// Initialize MPI
	MPI_Init(&argc, &argv);

	
	MPI_Comm world = MPI_COMM_WORLD;

	int worldSize, rank, procLen;
	char name[MPI_MAX_PROCESSOR_NAME];	

	MPI_Comm_size(world, &worldSize);
	MPI_Comm_rank(world, &rank);
	MPI_Get_processor_name(name, &procLen);
	printf("Hello world from rank %d with world size of %d and process name %s\n", rank, worldSize, name);
	
	MPI_Finalize();
	return 0;
}
