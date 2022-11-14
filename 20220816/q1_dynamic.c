#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>


int main(int argc, char ** argv) {
    int numThreads = atoi(argv[1]);
    int chunkSize = atoi(argv[2]);
    int limit = 10000;
    int oddSum = 0;
    int evenSum = 0;
    int i = 0;
    clock_t t = clock();

    #pragma omp parallel shared(evenSum, chunkSize) private(i) num_threads(numThreads)
    {
        #pragma omp parallel for schedule(dynamic, chunkSize) reduction(+ : evenSum)
        for (i = 0; i < limit; i += 2) evenSum += i;
    }

    #pragma omp parallel shared(oddSum, chunkSize) private(i) num_threads(numThreads)
    {
        #pragma omp parallel for schedule(dynamic, chunkSize) reduction(+ : oddSum)
        for (i = 1; i < limit; i += 2) oddSum += i;
    }
    
    t = clock() - t;
    
    printf(
        "static scheduling | %fs | %d threads | chunk size = %d | oddSum: %d | evenSum: %d ",
        ((double)t/CLOCKS_PER_SEC),
        numThreads,
        chunkSize,
        oddSum,
        evenSum
    );
    
    printf("\n\n");
    return 0;
}
