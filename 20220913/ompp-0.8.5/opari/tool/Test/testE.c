#include <stdio.h>

int main() {
  int i, j, k;

  #pragma omp parallel
  {
    #pragma omp for nowait
    for (i=0; i<4; ++i) {
      for (j=0; j<4; ++j) {
        printf("do %d\n", i);
      }
    }

    #pragma omp for nowait
    for (i=0; i<4; ++i) {
      for (j=0; j<4; ++j) {
        for (k=0; k<4; ++k) {
          printf("do %d\n", i);
        }
      }
    }

    #pragma omp for nowait
    for (i=0; i<4; ++i)
      for (j=0; j<4; ++j)
        printf("do %d\n", i);

    #pragma omp for nowait
    for (i=0; i<4; ++i)
      for (j=0; j<4; ++j)
        for (k=0; k<4; ++k)
          printf("do %d\n", i);

    #pragma omp for nowait
    for (i=0; i<4; ++i)
      for (j=0; j<4; ++j) {
        printf("do %d\n", i);
      }
  }
}
