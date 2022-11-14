/*
a. Write a program in MPI to create two processes in two
    different machines. Process 0 pings Process 1 and
    awaits for return ping using Non-blocking message
    passing routines. Execute your code on MPI cluster. 
*/

#include <stdio.h>
#include <mpi.h>

int main(){
    int rank, size;
    int tag, destination, count;
    int buffer;

    tag = 1234;
    destination = 1;
    count = 1;

    MPI_Status status;
    MPI_Request request = MPI_REQUEST_NULL;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        buffer=10;
        MPI_Isend(&buffer, count, MPI_INT, destination, tag, MPI_COMM_WORLD, &request);
    }
    if (rank == destination) {
        MPI_Irecv(&buffer, count, MPI_INT, 0, tag, MPI_COMM_WORLD, &request);
    }
    MPI_Wait(&request, &status);
    if (rank == 0) {
        printf("proc %d sent %d\n", rank, buffer);

    }else if (rank == destination) {
        printf("proc %d got %d\n", rank, buffer);
    }
    MPI_Finalize();
    return 0;
}