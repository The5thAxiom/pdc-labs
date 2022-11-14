/* 
 * ompp_region.c  */
/* @OMPP_HEADER@ */

#include <stdio.h>
#include <assert.h>
#ifndef _MSC_VER
#include <unistd.h>
#else
#pragma warning(disable : 4996)
#endif

#include "ompp.h"
#include "ompp_report.h"
#include "ompp_region.h"
#include "ompp_regstat.h"

#ifdef OMPP_USE_PAPI
#include "ompp_papi.h"
#endif


void ompp_region_init( ompp_region_t *reg )
{
  static unsigned next_number=1;

  reg->desc     = 0;
  reg->rstat    = 0;
  reg->type     = OMPP_UNSPECIFIED;
  reg->number   = next_number++;
  reg->nstats   = 0;
  /*  reg->par_base = 0; */
}


//
// get a region number that doesn't change between 
// differnet runs
//
int ompp_region_get_static_number( ompp_region_t *reg )
{
  int i, j;

  assert(reg);
  if( !(reg->desc) )
    return 0;

  for( i=0, j=0; i<POMP_MAX_ID; i++ )
    {
      if( pomp_rd_table[i] )
	j++;

      if( pomp_rd_table[i]==(reg->desc) )
	return j;
    }

  

  return 0;
}


ompp_region_t* ompp_get_region_by_number( unsigned num )
{
  ompp_region_t *reg, *res;
  lelem_t *elem;

  res=0;
  elem = ompp_region_list.head;
  while(elem) 
    {
      reg = (ompp_region_t*)elem->data;

      if( reg && reg->number==num )
	{
	  res=reg;
	  break;
	}
      elem=elem->next;
    }

  return res;
}



void ompp_region_enter( ompp_region_t *reg,
			ompp_reg_type_t type, 
			int tid )
{
  ompp_region_t *prevreg=0;
	
  assert(reg);
  assert(reg->rstat);

  /*
    fprintf(stderr, "[%d]: ENTER  %s %s %p\n", tid, 
	  ompp_region_full_name[reg->type],
	  ompp_region_full_name[type], reg );
  */

  
  //
  // maintain region stack
  //
  switch( type )
    {
      // 
      // only the master thread executes those events but 
      // we need to bring the region stack up-to-date for the
      // others as well:
      // 
    case OMPP_PARALLEL:
    case OMPP_PARALLEL_LOOP:
    case OMPP_PARALLEL_SECTIONS:
    case OMPP_PARALLEL_WORKSHARE:
      {
	int i; 

	if( tid!=0 )
	  {
	    fprintf(stderr, "Nested parallelism not supported yet!\n");
	    exit(1);
	  }
	// assert( tid==0 );

	rstack_enter( &ompp_rstack, reg, tid );

#ifdef OMPP_CONTROLFLOW
	if( ompp_rstack.previous[0] )
	  prevreg = ompp_rstack.previous[0]->reg;

	if( prevreg )
	  {
	    (ompp_rstack.current[0]->predcount[0][prevreg->number])++;
	  }
	else
	  {
	    (ompp_rstack.current[0]->predcount[0][0])++;
	  }
#endif /* OMPP_CONTROLFLOW */

	for( i=1;i<ompp_max_threads;i++ ) 
	  {
	    ompp_rstack.current[i]=ompp_rstack.current[0];
	  }
      }
      break;      

      //
      // those are simple cases where reg->type==type
      //
    case OMPP_ATOMIC:
    case OMPP_FLUSH:
    case OMPP_MASTER:
    case OMPP_CRITICAL:
      // we do not put LOCKs on the stack
      // case OMPP_LOCK: 
    case OMPP_SECTION:
    case OMPP_SECTIONS:
    case OMPP_SINGLE:
    case OMPP_REGION:
      if( reg->type==type )
	{
	  rstack_enter( &ompp_rstack, reg, tid );
	  
#ifdef OMPP_CONTROLFLOW 
	  if( ompp_rstack.previous[tid] )
	    prevreg = ompp_rstack.previous[tid]->reg;

	  if( prevreg )
	    {
	      (ompp_rstack.current[tid]->predcount[tid][prevreg->number])++;
	    }
	  else
	    {
	      (ompp_rstack.current[tid]->predcount[tid][0])++;
	    }
#endif /* OMPP_CONTROLFLOW */

	}
      break;

    case OMPP_TASK:
    case OMPP_TASKEXEC:
    case OMPP_UTASK:
    case OMPP_UTASKEXEC:
    case OMPP_TASKWAIT:
      rstack_enter( &ompp_rstack, reg, tid );
      /* TODO: fix control flow issue */

#ifdef OMPP_CONTROLFLOW
	  if( ompp_rstack.previous[tid] )
	    prevreg = ompp_rstack.previous[tid]->reg;

	  if( prevreg )
	    {
	      (ompp_rstack.current[tid]->predcount[tid][prevreg->number])++;
	    }
	  else
	    {
	      (ompp_rstack.current[tid]->predcount[tid][0])++;
	    }
#endif /* OMPP_CONTROLFLOW */

      break;

      //
      // here we have to assure that it is an explicit barrier
      // or an actual loop and not the same events occuring 
      // in a OMPP_PARALLEL* region
      //
    case OMPP_LOOP:
    case OMPP_BARRIER:
    case OMPP_WORKSHARE:
      if( reg->type==type )
	{

	  rstack_enter( &ompp_rstack, reg, tid );

#ifdef OMPP_CONTROLFLOW
	  if( ompp_rstack.previous[tid] )
	    prevreg = ompp_rstack.previous[tid]->reg;

	  if( prevreg )
	    {
	      (ompp_rstack.current[tid]->predcount[tid][prevreg->number])++;
	    }
	  else
	    {
	      (ompp_rstack.current[tid]->predcount[tid][0])++;
	    }
#endif /* OMPP_CONTROLFLOW */

	}
      break;
    }


#ifdef OMPP_USE_PAPI
  if( (OMPP_IS_PAPI_ON) &&
      !ompp_papi_initialized[tid] )
  {
      ompp_papi_thread_init(tid);
  }
#endif 
  

  if( (ompp_rstack.current[tid])->rstat )
    {
      regstat_on_enter( (ompp_rstack.current[tid])->rstat, 
			type, tid, 1 );
    }

  regstat_on_enter( reg->rstat, type, tid, 0 );
}


void ompp_region_exit( ompp_region_t *reg,
		       ompp_reg_type_t type,
		       int tid )
{
  assert(reg);
  assert(reg->rstat);
  assert(ompp_rstack.current[tid]);

  /*
  fprintf(stderr, "[%d]: EXIT   %s %s %s\n", tid, 
	  ompp_region_full_name[reg->type],
	  ompp_region_full_name[type],
	  reg->location);
  */
 
  if( (ompp_rstack.current[tid])->rstat )
    {
      regstat_on_exit( (ompp_rstack.current[tid])->rstat, 
		       type, tid, 1 );
    }

 
  regstat_on_exit( reg->rstat, type, tid, 0 );

  //
  // maintain region stack
  //
  switch( type )
    {
      // 
      // only the master thread executes those events but 
      // we need to bring the region stack up-to-date for the
      // others as well:
      // 
    case OMPP_PARALLEL:
    case OMPP_PARALLEL_LOOP:
    case OMPP_PARALLEL_SECTIONS:
    case OMPP_PARALLEL_WORKSHARE:
      {
	int i;

	if( tid!=0 )
	  {
	    fprintf(stderr, "Nested parallelism not supported yet!\n");
	    exit(1);
	  }	
	assert( tid==0 );
	
	for( i=0; i<ompp_max_threads; i++) 
	  {
	    rstack_exit( &ompp_rstack, reg, i );
	  }
      }
      break;
      
      //
      // those are simple cases where reg->type==type
      //
    case OMPP_ATOMIC:
    case OMPP_FLUSH:
    case OMPP_MASTER:
    case OMPP_CRITICAL:
      //case OMPP_LOCK: // locks are not put on the stack
    case OMPP_SECTION:
    case OMPP_SECTIONS:
    case OMPP_SINGLE:
    case OMPP_REGION:
      if( reg->type==type )
	{
	  rstack_exit( &ompp_rstack, reg, tid );
	}
      break;

    case OMPP_TASK:
    case OMPP_TASKEXEC:
    case OMPP_UTASK:
    case OMPP_UTASKEXEC:
    case OMPP_TASKWAIT:
      rstack_exit( &ompp_rstack, reg, tid );
      break;

      //
      // here we have to assure that it is an explicit barrier
      // or an actual loop 
      //
    case OMPP_LOOP:
    case OMPP_WORKSHARE:
    case OMPP_BARRIER:
      if( reg->type==type )
	rstack_exit( &ompp_rstack, reg, tid );
      break;    
    }

}


