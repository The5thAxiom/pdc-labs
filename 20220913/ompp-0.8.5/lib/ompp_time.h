/* 
 * ompp_time.h  */
/* @OMPP_HEADER@ */


#ifndef OMPP_TIME_H_INCLUDED
#define OMPP_TIME_H_INCLUDED


#include <sys/time.h>
#include <time.h>

/*
 use the macro OMPP_TIMESTAMP with a *double* 
 parameter argument. the returned value is time
 in seconds passed since some point of time 
 in the past
*/
#define OMPP_TIMESTAMP( time_ )         \
{                                       \
  struct timeval tv;                    \
  gettimeofday( &tv, NULL );            \
  time_=tv.tv_sec+tv.tv_usec*1.0e-6;    \
}

/*
 convenience function directly returning 
 the wallclock time
*/ 
double ompp_wtime(void);

/* user time and system time */
double ompp_utime(void);
double ompp_stime(void);


#endif /* OMPP_TIME_H_INCLUDED */
