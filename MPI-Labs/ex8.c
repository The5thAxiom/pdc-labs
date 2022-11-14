#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]){
    double starttime = MPI_Wtime(), endtime;
    int procRank, procNum;
    int message;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    
    if (procRank == 0) {
        message = 1550;
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("proc %d sent message %d to id:1\n", procRank, message);
    } else if (procRank == 1) {
        MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Recieved %d message by proc%d\n",message,procRank);
    }
    endtime = MPI_Wtime();
    MPI_Finalize();
    printf("proc %d took %f seconds\n", procRank, endtime - starttime); return 0;
}