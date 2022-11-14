#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, numProcs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    int num[5] = {1, 2, 3, 4, 5};
    int recvNum[5];

    if (rank == 0) {
        MPI_Send(&num, 5, MPI_INT, 1, 1, MPI_COMM_WORLD);
    } else if (rank == 1) {
        MPI_Recv(&recvNum, 5, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("recieved:\n");
        for (int i = 0; i < 5; i++) printf("%d, ", recvNum[i]);
    }

    MPI_Finalize();
}
