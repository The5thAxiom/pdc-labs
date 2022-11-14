/*
Write a program in OpenMP to find out the largest number in an array of
1000000 randomly generated numbers from 1 to 100000 using reduction
clause. Compare the versions of serial, parallel for and reduction clause.
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000000
#define MAX 100000

void findMinSerial(int ar[]) {
    int max = 0;
    for (int i = 0; i < N; i++) if (ar[i] > max) max = ar[i];

    printf("%d is the maximum element\n", max);
}

void findMinParallel(int ar[]) {
    int max = 0, i;
    
    #pragma omp parallel for shared(ar, max) private(i)
    for (i = 0; i < N; i++) {
        if (ar[i] > max) {
            #pragma omp critical
            {
                max = ar[i];
            }
        }
    }
    printf("%d is the maximum element\n", max);
}

void findMinReduction(int ar[]) {
    int max = 0, i;
    
    #pragma omp parallel for shared(ar) private(i) reduction(max: max)
    for (i = 0; i < N; i++) if (ar[i] > max) max = ar[i];

    printf("%d is the maximum element\n", max);
}

int main() {
    int ar[N];
    
    srand(clock());
    
    for (int i = 0; i < N; i++) ar[i] = rand() % MAX;
    
    double t = omp_get_wtime();
    findMinSerial(ar);
    t = omp_get_wtime() - t;
    printf("Serial execution took %fs\n", t);
    
    t = omp_get_wtime();
    findMinParallel(ar);
    t = omp_get_wtime() - t;
    printf("Parallel execution using a critical section took %fs\n", t);
    
    t = omp_get_wtime();
    findMinReduction(ar);
    t = omp_get_wtime() - t;
    printf("Parallel execution using a reduction took %fs\n", t);
    
    return 0;
}
