#include <omp.h>
#include <stdio.h>

int main() {
  int me, glob;

  #pragma omp parallel private(me,glob)
  {
    me = omp_get_thread_num();
    printf("hello from %d\n", me);

    #pragma omp single copyprivate(glob)
    {
      glob = me;
      printf("single1 from %d\n", me);
    }

    printf("hello from %d\n", me);

    #pragma omp single 
    {
      printf("single2 from %d\n", me);
    }

    #pragma omp single nowait
    {
      printf("single3 from %d\n", me);
    }

  }
}
