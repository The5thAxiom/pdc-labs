/*
 * ompp_papi.h  */
/* @OMPP_HEADER@ */

#ifndef OMPP_PAPI_H_INCLUDED
#define OMPP_PAPI_H_INCLUDED

#ifdef OMPP_USE_PAPI

#include <omp.h>
#include <papi.h>
#include "matheval/matheval.h"
#include "ompp.h"

#if ((PAPI_VERSION_MAJOR(PAPI_VERSION) == 3) && \
     (PAPI_VERSION_MINOR(PAPI_VERSION) >= 9))
/* figure out if PAPI is component papi, 
   set macro to use mutli-component papi    */
//#define OMPP_USE_CPAPI       (1)
#endif

#ifdef OMPP_USE_CPAPI
/* eventset info per component 
   - each thread has its own evtset 
*/
typedef struct 
{
  int *evtset;
  int nevents;
  int started;
}
compinfo_t;

#endif

extern omp_lock_t papi_lock;


void ompp_papi_initialize();
void ompp_papi_thread_init(int tid);
void ompp_papi_thread_stop(int tid);

void OMPP_papi_set_drange( void* addr, size_t size, char* name );

#endif /* OMPP_USE_PAPI */

#endif /* OMPP_PAPI_H_INCLUDED */
