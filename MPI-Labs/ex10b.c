/*
b. Write a MPI program to compute PI using “dartboard”
    technique for 1000 rounds by using reduction
    collective computation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

double getRandom() {
    return (double)(rand() % 1000000) / 500000 - 1;
}

int main(int argc, char *argv[]) {
    srand(clock());
    int procRank, procNum;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    int count = 0, total = 0;
    int finalCount = 0, finalTotal = 0;

    if (procRank != 0) {
        for (int i = 0; i < 10000; i++) {
            double x = getRandom();
            double y = getRandom();
            if ((x*x)+(y*y)< 1) count++;
            total++;
        }
        printf("P%d: %d/%d\n", procRank, count, total);
    }
    MPI_Reduce(&count, &finalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&total, &finalTotal, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (procRank == 0) {
        printf(
            "Throws inside circle = %d\nTotal throws = %d\npi = %f\n",
            finalCount,
            finalTotal,
            (double)finalCount/finalTotal * 4
        );
    }
    MPI_Finalize();
}