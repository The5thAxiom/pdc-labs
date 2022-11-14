/*
Write a multithreaded program using OpenMP to implement sequential and
parallel version of the Monte Carlo algorithm for approximating Pi. Compare
the results of sequential, loop-level parallelism and reduction clause with
10000000 samples.
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>

#define N 100000
#define M 100000

void serialMonteCarloPi(double xSamples[], double ySamples[]) {
    int counter = 0;
    for (int i = 0; i < N; i++) {
        double x = xSamples[i];
        double y = ySamples[i];

        if (x * x + y * y < 1) counter++;
    }
    printf("pi = %f\n", 4.0 * (double) counter / (double) N);
}

void parallelMonteCarloPi(double xSamples[], double ySamples[]) {
    int counter = 0, i;
    double x, y;

    #pragma omp parallel for shared(xSamples, ySamples, counter) private(i, x, y)
    for (i = 0; i < N; i++) {
        x = xSamples[i];
        y = ySamples[i];

        if (x * x + y * y < 1) {
            #pragma omp critical
            {
                counter++;
            }
        }
    }
    printf("pi = %f\n", 4.0 * (double) counter / (double) N);
}

void reductionMonteCarloPi(double xSamples[], double ySamples[]) {
    int counter = 0, i;
    double x, y;

    #pragma omp parallel for shared(xSamples, ySamples) private(i, x, y) reduction(+: counter)
    for (i = 0; i < N; i++) {
        x = xSamples[i];
        y = ySamples[i];

        if (x * x + y * y < 1) counter += 1;
    }
    printf("pi = %f\n", 4.0 * (double) counter / (double) N);
}

int main() {
    srand(clock());

    double xSamples[N], ySamples[N];
    for (int i = 0; i < N; i++) {
        xSamples[i] = (double)(rand() % M) / M;
        ySamples[i] = (double)(rand() % M) / M;
    }

    double t = omp_get_wtime();
    serialMonteCarloPi(xSamples, ySamples);
    t = omp_get_wtime() - t;
    printf("Serial execution took %fs\n\n", t);
    
    t = omp_get_wtime();
    parallelMonteCarloPi(xSamples, ySamples);
    t = omp_get_wtime() - t;
    printf("Parallel execution using a critical section took %fs\n\n", t);
    
    t = omp_get_wtime();
    reductionMonteCarloPi(xSamples, ySamples);
    t = omp_get_wtime() - t;
    printf("Parallel execution using a reduction took %fs\n", t);

}