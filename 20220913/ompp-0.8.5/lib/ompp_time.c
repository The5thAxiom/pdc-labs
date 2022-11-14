/*
 * ompp_time.c */ 
/* @OMPP_HEADER@ */

#include <stdio.h>
#include <sys/times.h>
#include <sys/resource.h>

#include "ompp_time.h"
#include "ompp.h"

double ompp_wtime(void)
{
  double time;
  OMPP_TIMESTAMP(time);
  return time;
}


double ompp_utime(void)
{
  double time;
  struct rusage ru;

  getrusage(RUSAGE_SELF, &ru);  
  time = ru.ru_utime.tv_sec+ru.ru_utime.tv_usec*1.0e-6;

  return time;
}


double ompp_stime(void)
{
  double time;
  struct rusage ru;

  getrusage(RUSAGE_SELF, &ru);  
  time = ru.ru_stime.tv_sec+ru.ru_stime.tv_usec*1.0e-6;

  return time;
}
