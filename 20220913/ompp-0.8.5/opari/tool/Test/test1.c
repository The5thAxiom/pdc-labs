#include <stdio.h>
#ifdef _OPENMP
   #include <omp.h>  //just testing
#endif

int main() {
  omp_lock_t l;

  printf("before...\n");

  #pragma omp parallel
  {{
    printf("parallel 1...\n");
  }} //end

  //#pragma omp parallel
  {
    //printf("parallel 1...\n");
  }

/*
  #pragma omp parallel
  {
    printf("parallel 1...\n");
  }
*/

  # /*complicated*/ pragma \
                    omp \
    /*more*/        parallel
  {
    printf("parallel 2...\n");
  }

  //omp_init_lock();
  omp_init_lock(&l);

  printf("after...\n");
}
