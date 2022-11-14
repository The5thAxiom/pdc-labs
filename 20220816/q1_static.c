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
    int tempEvenSum = 0;
    int tempOddSum = 0;
    int i = 0;
    clock_t t = clock();
    
    #pragma omp parallel shared(evenSum, chunkSize) private(i) num_threads(4)
    {
        #pragma omp parallel for schedule(dynamic, chunkSize)
        for (i = 0; i < limit; i += 2) {
            #pragma omp critical
            {
                evenSum += 1;
            }
        }
    }

    #pragma omp parallel shared(oddSum, chunkSize) private(i) num_threads(4)
    {
        #pragma omp parallel for schedule(dynamic, chunkSize)
        for (i = 1; i < limit; i += 2) {
            #pragma omp critical
            {
                oddSum += 1;
            }
        }
    }
    
    t = clock() - t;
    
    printf(
        "dynamic scheduling | %fs | %d threads | chunk size = %d | oddSum: %d | evenSum: %d ",
        ((double)t/CLOCKS_PER_SEC),
        numThreads,
        chunkSize,
        oddSum,
        evenSum
    );
    
    printf("\n\n");
    return 0;
}
