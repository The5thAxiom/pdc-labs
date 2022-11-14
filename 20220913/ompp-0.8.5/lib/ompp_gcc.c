/*
 * ompp_gcc.c  */
/* @OMPP_HEADER@ */

#ifdef __GNUC__

#include <stdio.h>
#include <omp.h>
#include <assert.h>

#include "utils/hashtab.h"
#include "ompp_region.h"
#include "ompp_pomp.h"
#include "ompp_gcc.h"

#include "ompp.h"


struct hashtab *gccfuncs;
omp_lock_t ompp_gcc_lock;

void ompp_gcc_initialize()
{
 
  omp_init_lock( &ompp_gcc_lock );
  gccfuncs = hashtab_create( 1024 );
}

     
     
    
void __cyg_profile_func_enter(void *thisp, void *callsite)
{
  ompp_region_t *reg=0;
  int tid;
  
  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }
  
  /* tid = omp_get_thread_num(); */
  OMPP_GET_THREAD_NUM(tid);

  reg = ompp_get_region_for_gccfunc(thisp);

  ompp_region_enter( reg, OMPP_REGION, tid );
}

void __cyg_profile_func_exit(void *thisp, void *callsite)
{
  ompp_region_t *reg=0;
  int tid;

  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }

  /* tid = omp_get_thread_num(); */
  OMPP_GET_THREAD_NUM(tid)

  reg = ompp_get_region_for_gccfunc(thisp);

  ompp_region_exit( reg, OMPP_REGION, tid );
}




ompp_region_t* ompp_get_region_for_gccfunc( void* func )
{
  ompp_region_t *reg=0;
  
  reg = (ompp_region_t*) hashtab_search( gccfuncs, func );

  if( !reg )
    {
      omp_set_lock( &ompp_gcc_lock );
      
      reg = (ompp_region_t*) hashtab_search( gccfuncs, func );
      if( !reg )
	{
	  reg = (ompp_region_t*)malloc( sizeof(ompp_region_t) );
	  ompp_region_init(reg);
	  
	  reg->number+=POMP_MAX_ID;
	  reg->type = OMPP_REGION;
	  reg->desc = (struct ompregdescr*)func; // ABUSE!
	  
	  sprintf( reg->location, "GCCFUNC (%p)",
		   func );
	  
	  reg->rstat = (regstat_t*) malloc(sizeof(regstat_t) );
	  assert(reg->rstat);
	  regstat_init( reg->rstat, reg, ompp_max_threads );

	  hashtab_insert( gccfuncs, func, reg );


	  omp_set_lock( &make_region_lock );
	  {
	    lelem_t *elem = (lelem_t*) malloc( sizeof(lelem_t) );
	    elem->data    = (void*)reg;
	    list_add_back( &ompp_region_list, elem );
	    ompp_region_count[reg->type]++;
	  }
	  omp_unset_lock( &make_region_lock );
	}
	 
      omp_unset_lock( &ompp_gcc_lock );
    }

  return reg;
}

#endif 

