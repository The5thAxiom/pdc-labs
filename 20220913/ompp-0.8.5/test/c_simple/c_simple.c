
#include <omp.h>
#include <stdio.h>

int main( int argc, char* argv[] )
{
#pragma omp parallel
  {
    int tid;
    
    tid = omp_get_thread_num();

    fprintf(stderr, "Thread %d of %d\n", tid, 
	    omp_get_num_threads() );
  }  
}
