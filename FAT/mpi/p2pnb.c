#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);
	int rank, numProcs;
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int num = 10, numR;
	
	printf("proc %d\n", rank);
	if (rank == 0) {
	} else if (rank == 1) {
	}

	MPI_Finalize();
	return 0;
}
