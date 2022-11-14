#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10

#define OMP_NUM_THREADS 4

/*
1. Write your own code snippet to demonstrate the following
    a. Barrier
    b. Master
    c. Single
    d. Critical
    e. Ordered
*/

void barrier() {
    printf("\na. Barrier:\n");
    int a[1000], b[1000], i = 0, sum = 0;

    for (i = 0; i < 1000; i++) {
        a[i] = rand() % 100;
        b[i] = rand() % 10;
    }

    #pragma omp parallel private(i)
    {
        for (i = 0; i < 1000; i++) {
            a[i] = a[i] - b[i];
        }

        #pragma omp barrier

        #pragma omp for reduction(+ : sum)
        for (i = 0; i < 1000; i++)
        {
            sum += a[i];
        }
    }
    printf("sum: %d\n", sum);
}

void master() {
    printf("\nb. Master\nwithout 'master':\n");
    #pragma omp parallel
    {
        printf("hello, from thread %d\n", omp_get_thread_num());
    }
    
    printf("\nwith master:\n");
    #pragma omp parallel
    {
        #pragma omp master
        {
            printf("hello, from thread %d\n", omp_get_thread_num());
        }
    }
}

void single() {
    printf("\nc. Single:\n");
    int a=0, b=0;
    #pragma omp parallel num_threads(4)
    {
        #pragma omp single
        a++;
        #pragma omp critical
        b++;
    }
    printf("single: %d | critical: %d\nsingle runs once, critical is run once per thread\n", a, b);
}

void critical() {
    printf("\nd. Critical:\n");
    
    int i; int max; int a[N];
    for (i = 0; i < N; i++) {
        a[i] = rand();
        printf(
            "a[%d] = %d\tthread no %d\n",
            i,
            a[i],
            omp_get_num_threads()
        );
    }
    max = a[0];
    #pragma omp parallel for
    for (i = 1; i < N; i++) {
        if (a[i] > max) {
            #pragma omp critical
            {
                if (a[i] > max) max = a[i];
            }
        }
    }
    printf("\nmax = %d\t%d threads\n", max, omp_get_num_threads()); 
}

void ordered() {
    printf("\ne. Ordered:\nwithout ordered:\n");
    
    int i = 0;
    int n = 10;
    
    #pragma omp parallel shared(n) private(i)
    {
        #pragma omp for
        for (i = 0; i < n; i++) {
            printf("thread %d at index %d\n", omp_get_thread_num(), i);
        }
    }
    
    printf("\nwith ordered:\n");
    
    #pragma omp parallel shared(n) private(i)
    {
        #pragma omp for ordered
        for (i = 0; i < n; i++) {
            #pragma omp ordered
            {
                printf("thread %d at index %d\n", omp_get_thread_num(), i);
            }
        }
    }
}

int main() {
    barrier();
    master();
    single();
    critical();
    ordered();

    printf("\n");
    return 0;
}
