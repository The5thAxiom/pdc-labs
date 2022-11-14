/*
 * ompp_overheads.h  */ 
/* @OMPP_HEADER@ */


#ifndef OMPP_OVERHEADS_H_INCLUDED
#define OMPP_OVERHEADS_H_INCLUDED

#include "ompp_region.h"
#include "utils/list.h"

typedef struct overheads
{
  // parallel region to which the overheads refer
  ompp_region_t *reg;
  
  double sync;   // synchronization overhead
  double imbal;  // imbalance overhead
  double limpar; // overhead due to limited parallelism
  double mgmt;   // thread management overhead

#ifdef OMPP_MPI
  double mpit;    // time in MPI routines 
#endif

  double sumovhd;  // sum of all overheads
  double wallt;    // wallclock time

#ifdef OMPP_USE_PAPI
  long long ctr[OMPP_PAPI_MAX_CTRS];
#endif 

}
overheads_t;

void overheads_init( overheads_t *ovhds );

void analyze_overheads( dllist_t *reglist, 
			rstack_t *rstack, 
			overheads_t *ovhds, 
			unsigned npara );

int ovhd_compare_by_wallclock( const void *p1, const void *p2 );
int ovhd_compare_by_overheads( const void *p1, const void *p2 );

#endif /* OMPP_OVERHEADS_H_INCLUDED */

