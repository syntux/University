#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<mpi.h>
#include<time.h>
#include<string.h>

// Use macros for some speed!
// this a "function" that takes four arguments
#define INDEX(i,j,n,m) i*m + j
#define ACCESS(A,i,j) A.data[INDEX(i,j,A.rows,A.cols)]

// if you would rather work with matrix* variables
//#define ACCESS(A,i,j) A->data[INDEX(i,j,A->rows,A->cols)]

// make an array object!
//
int main(int argc, char** argv){
  MPI_Init(NULL,NULL);

  MPI_Comm world = MPI_COMM_WORLD;
  int rank,worldSize;
  MPI_Comm_size(world,&worldSize);
  MPI_Comm_rank(world,&rank);



	char a[3];


  MPI_Finalize();
  return 0;
}
















