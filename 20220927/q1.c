#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Request reqs[2];
    MPI_Status stats[2];

    int n;
    if (rank == 0) {
        n = -1;
        MPI_Isend(&n ,1 ,MPI_INT ,1 ,0 ,MPI_COMM_WORLD, &reqs[0]);
        printf("ham ye (%d) bhej rahen hain p2 ko\n", n);
    } else if (rank == 1) {
        MPI_Irecv(&n ,1 ,MPI_INT ,0 ,0 ,MPI_COMM_WORLD , &reqs[1]);
        printf("p1 ne ye diya: %d\n", n);
    }

    MPI_Waitall(2, reqs ,stats);
    MPI_Finalize();

    return 0;
}