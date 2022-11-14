#include <stdio.h>
#include <time.h>

#define VECTOR_SIZE 100000

int main() {
    // make the vectors
    int a[VECTOR_SIZE], b[VECTOR_SIZE], c[VECTOR_SIZE];
    for (int i = 0; i < VECTOR_SIZE; i++) {
        a[i] = VECTOR_SIZE - i;
        b[i] = i;
    }
    
    // serially add the vectors
    clock_t tSerial = clock();
    for (int i = 0; i < VECTOR_SIZE; i++) {
        c[i] = a[i] + b[i];
    }
    tSerial = clock() - tSerial;

    // show the output
    printf(
        "Serial addition took %f seconds\n",
        ((double) tSerial)/CLOCKS_PER_SEC
    );

    return 0;
}
