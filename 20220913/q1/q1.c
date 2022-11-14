#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/*
2. Write a multithreaded program using OpenMP for computing a
matrix-matrix product for a large dimension.
Use the OMP_NUM_THREADS environment variable to control
the number of threads and plot the performance with varying numbers
of threads (4,8 and 16). Consider four cases in which

i) Only the outermost loop is parallelized
ii) The outer two loops are parallelized
iii) All three loops are parallelized
iv) Use collapse clause
What is the observed result from these four cases?
Record your run times using omp_get_wtime() routine. 
*/

typedef int ** matrix;
#define N 1100 // the size of the matrices

void multiply(matrix A, matrix B, matrix ans, int numThreads) {
    int i = 0, j = 0, k = 0;
    #pragma omp parallel for shared(A, B, ans) private(i, j, k) num_threads(numThreads)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                ans[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void print(matrix A) {
    for (int i = 0; i < N; i++) {
        printf("%s", i == 0 ? "[\n " : " ");
        for (int j = 0; j < N; j++) {
            printf(
                "%s%d%s",
                j == 0 ? "[" : " ",
                A[i][j],
                j == N - 1 ? "]" : ","
            );
        }
        printf("%s", i == N - 1 ? "\n]\n" : ",\n");
    }
}

int main(int argc, char *argv[]) {
    int numThreads = atoi(argv[1]);
    matrix A, B, C;

    A = (int **)calloc(N, sizeof(int*));
    B = (int **)calloc(N, sizeof(int*));
    C = (int **)calloc(N, sizeof(int*));

    for (int i = 0; i < N; i++) {
        A[i] = (int *) calloc(N, sizeof(int));
        B[i] = (int *) calloc(N, sizeof(int));
        C[i] = (int *) calloc(N, sizeof(int));
        
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
            C[i][j] = 0;
        }
    }

    // print(A);
    // print(B);

    double t = omp_get_wtime();

    multiply(A, B, C, numThreads);

    t = omp_get_wtime() - t;

    // print(C);

    printf("took %f seconds\n", t);

    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
    }
    free(A);
    free(B);
    free(C);

    printf("\n");
    return 0;
}