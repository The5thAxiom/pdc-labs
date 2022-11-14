#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <time.h>

#define MAX_SLEEP 10

omp_lock_t lock;
int cr = 0;

void reader(int i) {
    #pragma omp critical
    {
        cr++;
        if (cr == 1) {
            omp_set_lock(&lock);
            printf("lock set by reader %d \n", i);
        }
    }

    printf(
        "reader %d (on thread %d) is reading\n",
        i,
        omp_get_thread_num()
    );

    sleep(rand() % MAX_SLEEP);

    #pragma omp critical
    {
        cr--;
        if (cr == 0) {
            omp_unset_lock(&lock);
            printf("lock unset by reader %d\n", i);
        }
    }
}

void writer(int i) {
    omp_set_lock(&lock);
    printf("lock set by writer %d\n", i);

    printf(
        "writer %d (on thread %d) is writing\n",
        i,
        omp_get_thread_num()
    );

    sleep(rand() % MAX_SLEEP);

    omp_unset_lock(&lock);
    printf("lock unset by writer %d\n", i);
}

int main(int argc, char *argv[]) {
    printf("Readers-writers in parallel\n");

    srand(clock());
    omp_init_lock(&lock);

    #pragma omp parallel sections num_threads(8)
    {
        #pragma omp section
        {
            writer(0);
        }
        #pragma omp section
        {
            reader(0);
        }
        #pragma omp section
        {
            reader(1);
        }
        #pragma omp section
        {
            reader(2);
        }
        #pragma omp section
        {
            writer(1);
        }
        #pragma omp section
        {
            writer(2);
        }
        #pragma omp section
        {
            reader(3);
        }
        #pragma omp section
        {
            reader(4);
        }
    }

    return 0;
}
