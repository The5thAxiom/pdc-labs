/*
Write a program in OpenMP to find out the standard deviation of 1000000
randomly generated numbers using reduction clause. Document the
development versions of serial, parallel for and reduction clause.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#define N 1000000
#define MAX 100000

void sdSerial(int ar[]) {
    double mean = 0;
    for (int i = 0; i < N; i++) mean += ar[i];
    mean /= N;

    double runningSum = 0;
    for (int i = 0; i < N; i++) {
        runningSum += pow(ar[i] - mean, 2);
    }

    double sd = sqrt(runningSum / N);
    printf("sd = %f\n", sd);
}

void sdParallel(int ar[]) {
    double mean = 0;
    int i = 0;

    #pragma omp parallel for shared(ar, mean) private(i)
    for (i = 0; i < N; i++) {
        #pragma omp critical
        {
            mean += ar[i];
        }
    }
    mean /= N;

    double runningSum = 0;

    #pragma omp parallel for shared(runningSum, mean) private(i)
    for (i = 0; i < N; i++) {
        #pragma omp critical
        {
            runningSum += pow(ar[i] - mean, 2);
        }
    }

    double sd = sqrt(runningSum / N);
    printf("sd = %f\n", sd);
}

void sdReduction(int ar[]) {
    double mean = 0;
    int i = 0;

    #pragma omp parallel for shared(ar) private(i) reduction(+ : mean)
    for (i = 0; i < N; i++) mean += ar[i];
    mean /= N;

    double runningSum = 0;

    #pragma omp parallel for shared(ar, mean) private(i) reduction(+ : runningSum)
    for (i = 0; i < N; i++) runningSum += pow(ar[i] - mean, 2);

    double sd = sqrt(runningSum / N);
    printf("sd = %f\n", sd);
}

int main() {
    int ar[N];
    
    srand(clock());
    
    for (int i = 0; i < N; i++) ar[i] = rand() % MAX;

    double t = omp_get_wtime();
    sdSerial(ar);
    t = omp_get_wtime() - t;
    printf("Serial execution took %fs\n\n", t);
    
    t = omp_get_wtime();
    sdParallel(ar);
    t = omp_get_wtime() - t;
    printf("Parallel execution using a critical section took %fs\n\n", t);
    
    t = omp_get_wtime();
    sdReduction(ar);
    t = omp_get_wtime() - t;
    printf("Parallel execution using a reduction took %fs\n", t);

    return 0;
}
