/*
b. Write a program in MPI to create 10 tasks.
    Construct a ring topology to exchange message to its
    nearest neighbour in the ring using blocking massage
    passing routines. Execute your code on MPI cluster.
*/

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int myid, numprocs, left, right;
    int buffer = 1550, buffer2;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    
    MPI_Request request[2];
    MPI_Status status;

    right = (myid+ 1) % numprocs;

    left = myid-1; if (left < 0)
    left = numprocs-1;

    MPI_Isend(&buffer, 1, MPI_INT, right, 123, MPI_COMM_WORLD, &request[0]);
    printf("proc %d to proc %d sent number: %d\n", myid, right, buffer);
    MPI_Irecv(&buffer2, 1, MPI_INT, left, 123, MPI_COMM_WORLD, &request[1]);
    MPI_Wait(&request[0], &status);
    MPI_Wait(&request[1], &status);
    printf("proc %d from proc %d, received number: %d\n",myid,left,buffer2);
    MPI_Finalize();
    return 0;
}