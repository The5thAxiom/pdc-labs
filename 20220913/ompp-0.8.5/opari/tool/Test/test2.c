#include <stdio.h>
#ifdef _OPENMP
  #include <omp.h>
#endif

int main() {
  int i;

 #pragma omp parallel
 {
   printf("parallel\n");
   #pragma omp for nowait
   for(i=0; i<4; ++i) {
     printf("do nowait %d\n", i);
   }

   #pragma omp barrier

   #pragma omp for
   for(i=0; i<4; ++i) {
     printf("do %d\n", i);
   }

   #pragma omp sections nowait
   {
     #pragma omp section
     printf("section nowait 1\n");
     #pragma omp section
     { printf("section nowait 2\n"); }
   }

   #pragma omp master
   {
     printf("master\n");
   }

   #pragma omp critical
   {
     printf("critical\n");
   }

   #pragma omp critical(foobar)
   {
     printf("critical\n");
   }

   #pragma omp atomic
   /* -------------- */
   /* do this atomic */
   i += 1;
   /* -------------- */

   #pragma omp single
   {
     printf("single\n");
   }

   #pragma omp sections
   {
     #pragma omp section
     { printf("section 1\n"); }
     #pragma omp section
     printf("section 2\n");
   }
 }

 printf("sequential1\n");

 #pragma omp parallel for
 for(i=0; i<4; ++i) {
   printf("pdo %d\n", i);
 }

 printf("sequential2\n");

 #pragma omp parallel sections
 {
   #pragma omp section
   printf("psection 1\n");
   #pragma omp section
   printf("psection 2\n");
 }
}
