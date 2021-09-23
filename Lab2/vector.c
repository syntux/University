#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define INDEX(n, m, i, j) m*i + j

int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);

	int rank, worldSize;

	MPI_Comm world = MPI_COMM_WORLD;

	MPI_Comm_size(world, &worldSize);	
	MPI_Comm_rank(world, &rank);	

	
	int N = 2, i;
	
	int *vectorBlock = NULL;
	int *vectorBlock2 = NULL;
	int *vector1 = NULL;
	int *vector2 = NULL;

	int *sendcounts = NULL; 

	int num, dsum = 0;
	int *displ = NULL;

	int start = N / worldSize;
	int remainder = N - (start * worldSize);
	
	sendcounts = malloc(worldSize*sizeof(int));
	//	int *sendcounts; 
	//	sendcounts = malloc(worldSize*sizeof(int));
	for (i = 0; i < worldSize; i++) {
		sendcounts[i] = start;
	}
	i = 0;
	while (remainder != 0) {
		sendcounts[i]++;	
		i++;
		remainder--;
	}

	displ = malloc(worldSize*sizeof(int));
	
	displ[0] = 0;
	for (int i = 1; i < worldSize; i++) {	
		displ[i] = displ[i-1] + sendcounts[i-1];
		//printf("displ %d i %d\n", displ[i], i);
	}
	/*
	for (int i = 1; i < worldSize; i++) {
		displ[i] = dsum;
		dsum += sendcounts[i];
	}
	*/
	if (rank == 0) {
		srand(time(0));
		
		int i;
		//int n = 1; // Rows
		//int m = N; // Cols

		//vector1 = malloc(worldSize*N*sizeof(int));
		//vector2 = malloc(worldSize*N*sizeof(int));
		vector1 = malloc(N*sizeof(int));
		vector2 = malloc(N*sizeof(int));
		//for (i = 0; i < N*worldSize; i++) {
		for (i = 0; i < N; i++) {
			vector1[i] = rand() % 10;
			printf("1 %d \n", vector1[i]);
			vector2[i] = rand() % 10;
			printf("2 %d \n", vector2[i]);

		}
		puts("");
	}

	/*
	MPI_Scatter(
		&sendcounts[rank],
		1,
		MPI_INT,
		&num,
		1,
		MPI_INT,
		0,
		world	
	);	
	*/

//	printf("Rank %d and num: %d\n", rank, num);
	// Not N
	
	num = sendcounts[rank];
	vectorBlock = malloc(num*sizeof(int));
	vectorBlock2 = malloc(num*sizeof(int));


	MPI_Scatterv(
		vector1,	//sendbuf
		sendcounts, //sendcounts
		displ, 	//displs
		MPI_INT,	//sendtype
		vectorBlock,//recbuff
		num,		//recvcount
		MPI_INT,	//recvtype
		0,			// root
		world		// comm	
	);


	MPI_Scatterv(
		vector2,	//sendbuf
		sendcounts, //sendcounts
		displ, 	//displs
		MPI_INT,	//sendtype
		vectorBlock2,//recbuff
		num,		//recvcount
		MPI_INT,	//recvtype
		0,			// root
		world		// comm	
	);

	int sum = 0, result;
	for (int i = 0; i < num; i++) {
		printf("Rank %d element %d is %d\n", rank, i, vectorBlock[i]);	
		printf("2 Rank %d element %d is %d\n", rank, i, vectorBlock2[i]);	
		sum += vectorBlock[i] * vectorBlock2[i];
	}

	printf("Sum %d\n", sum);

	MPI_Reduce(
		&sum,	//sendbuf
		&result, //recvbuf
		1,		//count
		MPI_INT, //datatype
		MPI_SUM, //op
		0,		//root
		world	//comm
	);

	

	free(vectorBlock);
	free(vectorBlock2);

	
	free(sendcounts);
	if (rank == 0) {
		free(vector1);
		free(vector2);

		free(displ);

		printf("Reduced: %d\n", result);
	}

	MPI_Finalize();
}


