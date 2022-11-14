#include <stdio.h>

int main() {
  int i;

  #pragma omp parallel
  {
    printf("parallel\n");
    #pragma omp for nowait
    for (i=0; i<4; ++i) printf("do ; ; ; nowait %d\n", i);

    #pragma omp barrier

    #pragma omp \
	   /* unnütz */ \
	   /* mehr unnütz */ \
	   for /* KEIN nowait */
    for (i=0; i<4; ++i) {
      printf("do nowait? %d\n", i);
    }

    #pragma omp for
    for (i=0; i<4; ++i) {
      printf("do %d\n", i);
    }
  }
}
