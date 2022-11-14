/*
a. Write a program in MPI to generate 'n' random float
    numbers and send 'k' of those to each node and make
    them compute the average and send it back to the
    master which computes the average of those averages.
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]){
    int procRank, procNum;
    int n, k, numSend, numRecv;
    double avg;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    k = 4;
    n = k * procNum;

    if (procRank == 0) {
        for (int dest = 1; dest < procNum; dest++){
            for (int j = 0; j < k; j++) {
                numSend = rand() % 100;
                printf("send: P%d(%d)->P%d\n", procRank, numSend, dest);
                MPI_Send(&numSend, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            }
        }
    } else {
        int count = 0, sum = 0, from = 0;
        for (int i = 0; i < k; i++){
            MPI_Recv(&numRecv, 1, MPI_INT, from, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            count++;
            sum += numRecv;
            printf("recv: P0->P%d(%d) [%d/%d]\n", procRank, numRecv, count, k);
        }
        avg = (double)sum / count;
        printf("calc: P%d_average=%3f\n", procRank, avg);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (procRank != 0) {
        MPI_Send(&avg, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        printf("P%d(%f)->P0\n", procRank, avg);
    } else {
        int count = 0;
        double sum = 0.0;
        for (int i = 1; i < procNum; i++) {
            MPI_Recv(&avg, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("P%d->P0(%f) [%d/%d]\n", i, avg, count, procNum-1);
            count++;
            sum += avg;
        }
        printf("\nFinal average = %f\n", sum / count);
    }

    MPI_Finalize();
    return 0;
}