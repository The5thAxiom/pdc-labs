#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define VECTOR_SIZE 100000

int main(int argc, char * argv[]) {
    // make the vectors
    int a[VECTOR_SIZE], b[VECTOR_SIZE], c[VECTOR_SIZE];
    for (int i = 0; i < VECTOR_SIZE; i++) {
        a[i] = VECTOR_SIZE - i;
        b[i] = i;
    }

    int nThreads = atoi(argv[1]);

    // paralelly add the vectors:
    
    clock_t tPar = clock();
    
    // the part of the vector one thread will access
    int slice_size = VECTOR_SIZE / nThreads;

    int slice_start, slice_end;
    int tid;

    // make threads
    omp_set_num_threads(nThreads);
    #pragma omp parallel private (tid, slice_start, slice_end)
    {
        // allot a slice to the particular thread
        tid = omp_get_thread_num();
        slice_start = tid * slice_size;
        slice_end = slice_start + slice_size;

        // perform addition for the elements in the allotted slice_size
        for (int i = slice_start; i < slice_end; i++) {
            c[i] = a[i] + b[i];
        }
    }

    tPar = clock() - tPar;
    
    // show the output
    printf(
        "Parallel addition took %f seconds with %d threads\n",
        ((double) tPar)/CLOCKS_PER_SEC,
        nThreads
    );

    return 0;
}
