// using compiler directives

#include<stdio.h>
#include<omp.h>

// compile using: `gcc filename -fopenmp`

int main(int argc, char *argv[]) {
    int tid, numThreads;
    
    # pragma omp parallel private (tid, numThreads) num_threads(100)
    {
        tid = omp_get_thread_num();
        printf("welcome to PDC %d\n", tid);
        
        if (tid == 0) {
            numThreads = omp_get_num_threads();
            printf("%d threads have been created\n", numThreads);
        }
    }
    return 0;
}
