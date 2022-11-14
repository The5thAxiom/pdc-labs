/*
 * ompp_incremental.c  */
/* @OMPP_HEADER@ */

#include <signal.h>
#include <stdlib.h>
#include <string.h>

#ifndef _MSC_VER
#include <sys/time.h>
#endif

#include "ompp_report.h"
#include "ompp_incremental.h" 

void OMPP_Dump()
{
  print_report();
}

void ompp_dump_()
{
  print_report();
}


void ompp_handle_timer(int signum)
{
  ompp_done=0;
  print_report();
}


void ompp_profile_over_time_initialize(void)
{
  char *s;
  double d;
  unsigned ival;

  struct sigaction sa;
  struct itimerval timer;
  struct itimerval timer2;

  if( (s=getenv("OMPP_DUMP_INTERVAL"))!=0 &&
      ((d=atof(s))>0.0) )
    {
      ival = (unsigned) (d*1000000.0);

      if( (d<0.1) )
	{
	  fprintf(stderr, "ompP: profiling interval %f secs too small\n", d);
	  return;
	}

      fprintf(stderr, "ompP: using a profiling interval of %u usecs \n", ival);
    
      /* Install timer_handler as the
	 signal handler for SIGALRM. */
    
      memset (&sa, 0, sizeof (sa));
      sa.sa_handler = &ompp_handle_timer;
      sigaction (SIGPROF, &sa, NULL);
      
      /* Configure the timer to
	 expire after ival microsecs ... */
      timer.it_value.tv_sec = 0;
      timer.it_value.tv_usec = ival;
      
      /* ... and every ival microsecs after that. */
      timer.it_interval.tv_sec = 0;
      timer.it_interval.tv_usec = ival;
      
      /* Start a real timer */
      setitimer (ITIMER_PROF, &timer, 0);
    }
}
