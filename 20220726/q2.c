#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

void * void_function(void *message) {}

int main(int argc, char * argv[]) {
    pthread_t * threads;
    int num_threads = atoi(argv[1]); // because the cli argument is an ASCII code

    threads = (pthread_t *) calloc(num_threads, sizeof(pthread_t));

    clock_t t = clock();

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, void_function, NULL);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    t = clock() - t;

    printf(
        "took %f seconds to make %d threads\n",
        ((double) t)/CLOCKS_PER_SEC,
        num_threads
    );

    free(threads);

    return 0;
}
