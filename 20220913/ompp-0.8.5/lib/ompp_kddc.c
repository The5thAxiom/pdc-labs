
#include "ompp_kddc.h"

#include <stdio.h>

void __kddc_start() 
{
  fprintf(stderr, "__kddc_start\n");
}

int __kddc_run( int *a)
{
  //int tid=omp_get_thread_num();
  fprintf(stderr, "__kddc_run tid=%d\n", *a);
  return 4;
}

void __kddc_push_parallel()
{
  fprintf(stderr, "__kddc_push_parallel\n");
}

void __kddc_get_active()
{
  fprintf(stderr, "__kddc_get_active\n");
}

void __kddc_set_active()
{
  fprintf(stderr, "__kddc_set_active\n");
}

void __kddc_pop_parallel()
{
  fprintf(stderr, "__kddc_pop_parallel\n");
}

void __kddc_reset_active()
{
  fprintf(stderr, "__kddc_reset_active\n");
}

