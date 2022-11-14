#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <string.h>

#define N 10000
#define NUM_THREADS 4

/*
2. Write a parallel program to sort N elements in an array using OpenMP
    i. Bubble Sort
    ii. Quick Sort
*/

void serialBubbleSort(int array[]) {
    int i, temp;
    for (int times = 0; times < N; times++) {
        for (int i = 0; i < N - 1; i ++) {
            if (array[i] > array[i + 1]) {
                temp = array[i];
                array[i] = array[i + 1];
                array[i + 1] = temp;
            }
        }
    }
}

void parallelBubbleSort(int array[]) {
    int i, temp;

    for (int times = 0; times < N; times++) {
        #pragma omp parallel num_threads(NUM_THREADS) shared(array) private(i, temp)
        {
            // even phase
            #pragma omp for schedule(static)
            for (int i = 0; i < N - 1; i += 2) {
                if (array[i] > array[i + 1]) {
                    temp = array[i];
                    array[i] = array[i + 1];
                    array[i + 1] = temp;
                }
            }

            // odd phase
            #pragma omp for schedule(static)
            for (int i = 1; i < N - 1; i += 2) {
                if (array[i] > array[i + 1]) {
                    temp = array[i];
                    array[i] = array[i + 1];
                    array[i + 1] = temp;
                }
            }
        }
    }
}

int partition (int a[], int start, int end) {
    int pivot = a[end];
    int i = start - 1;
    for (int j = start; j <= end - 1; j++) {
        if (a[j] < pivot) {
            i++;
            int t = a[i];
            a[i] = a[j];
            a[j] = t;
        }
    }
    int t = a[i+1];
    a[i+1] = a[end];
    a[end] = t;
    return (i + 1);
}

void serialQuickSortHelper(int a[], int start, int end){
    if (start < end){
        int p = partition(a, start, end);
        serialQuickSortHelper(a, start, p - 1);
        serialQuickSortHelper(a, p + 1, end);
    }
}

void serialQuickSort(int array[]) {
    serialQuickSortHelper(array, 0, N - 1);
}

void parallelQuickSortHelper(int a[], int start, int end) {
    if (start < end) {
        int p = partition(a, start, end);
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelQuickSortHelper(a, start, p - 1);
            }
            #pragma omp section
            {
                parallelQuickSortHelper(a, p + 1, end);
            }
        }
    }
}

void parallelQuickSort(int array[]) {
    parallelQuickSortHelper(array, 0, N - 1);
}

void print(int a[]) {
    for (int i = 0; i < N; i++) {
        printf(
            "%s%d%s",
            i == 0 ? "[" : "",
            a[i],
            i == N - 1 ? "]" : ", "
        );
    }
    printf("\n");
}

void testAlgorithm(char name[], void (*algorithm)(int *), int array[]) {
    clock_t t = clock();

    algorithm(array);

    t = clock() - t;

    // print(array);

    printf(
        "%s took %f seconds\n",
        name,
        ((double)t) / CLOCKS_PER_SEC
    );
    printf("\n");
}

int main(int argc, char ** argv) {
    int a[N], b[N], c[N], d[N];
    srand(time(NULL));

    // generate random numbers to sort
    for (int i = 0; i < N; i++) {
        int r = rand() % 1000;
        a[i] = r;
        b[i] = r;
        c[i] = r;
        d[i] = r;
    }

    // show the random array
    // printf("Original Array:\n");
    // print(a);
    // printf("\n");

    // run each algorithm
    testAlgorithm("Serial Bubble Sort", serialBubbleSort, a);
    testAlgorithm("Parallel Bubble Sort", parallelBubbleSort, b);
    testAlgorithm("Serial Quicksort", serialQuickSort, c);
    testAlgorithm("Parallel Quicksort", parallelQuickSort, d);

    return 0;
}
