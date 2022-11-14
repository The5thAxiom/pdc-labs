/*
 * ompp_regstat.c  */
/* @OMPP_HEADER@ */


#include "ompp.h"
#include "ompp_region.h"
#include "ompp_regstat.h"
#include "ompp_time.h"
#include "ompp_mpi.h"

#ifdef OMPP_USE_PAPI
#include <papi.h>
#include "matheval/matheval.h"
#endif 

#include <stdio.h>
#include <assert.h>

#ifndef _MSC_VER
#include <sys/time.h>
#endif


#ifdef OMPP_USE_PAPI

long long mstat_sumctr( mstat_t *mstat, unsigned c )
{
  int i;
  long long sum=0;

  for( i=0; i<mstat->nctr; i++ )
    {
      sum+=mstat->ctr[c][i];
    }

  return sum;
}

#endif

void mstat_enter( mstat_t *mstat, unsigned tid, perfdata_t pd )
{
  int i;

  /*
  fprintf(stderr, "mstat_enter %p %d %d\n", mstat, tid, mstat->rlevel[tid]);
  */

  if( mstat->rlevel[tid]<OMPP_MAX_RECURSION ) 
    {
      if( tid<mstat->nexect )
	mstat->exect_enter[mstat->rlevel[tid]][tid]=pd.time;
      
#ifdef OMPP_MPI
      if( tid<mstat->nctr )
	{
	  /*
	    fprintf(stderr, "mstat_enter %d %f %d %d %d %d\n", 
	    tid, pd.mpi.time, pd.mpi.nsend, pd.mpi.bsend,
	    pd.mpi.nrecv, pd.mpi.brecv );
	  */
	  
	  mstat->time_enter[tid]  = pd.mpi.time;
	  mstat->nsend_enter[tid] = pd.mpi.nsend;
	  mstat->nrecv_enter[tid] = pd.mpi.nrecv;
	  mstat->ncoll_enter[tid] = pd.mpi.ncoll;
	  mstat->bout_enter[tid]  = pd.mpi.bout;
	  mstat->bin_enter[tid]   = pd.mpi.bin;
	}
#endif 
      
#ifdef OMPP_USE_PAPI
      if( tid<mstat->nctr )
	{
	  for( i=0; i<OMPP_PAPI_MAX_CTRS; i++ )
	    {
	      /* fprintf(stderr, "mstat_enter i=%d tid=%d ctr=%lld\n", i, tid, pd.ctr[i]); */
	      mstat->ctr_enter[i][mstat->rlevel[tid]][tid]=pd.ctr[i];
	    }
	}
#endif
    }

  // todo check if over max rlevel
  mstat->rlevel[tid]++;
}


void mstat_exit( mstat_t *mstat, unsigned tid, perfdata_t pd )
{
  int i;

  (mstat->rlevel[tid])--;

  if( mstat->rlevel[tid]<OMPP_MAX_RECURSION ) 
    {
      if( tid<mstat->nexecc )
	mstat->execc[tid]++;
      
      if( tid<mstat->nexect ) 
	{
	  mstat->exect[tid]+=(pd.time-mstat->exect_enter[mstat->rlevel[tid]][tid]);
	  mstat->exect_enter[mstat->rlevel[tid]][tid]=0.0;
	}
      
#ifdef OMPP_MPI
      if( tid<mstat->nctr )
	{
	  /*
	    fprintf(stderr, "mstat_exit %d %f %d %d %d %d\n", 
	    tid, pd.mpi.time, pd.mpi.nsend, pd.mpi.bsend,
	    pd.mpi.nrecv, pd.mpi.brecv );
	  */
	  
	  mstat->time[tid]  += pd.mpi.time  - mstat->time_enter[tid];
	  mstat->nsend[tid] += pd.mpi.nsend - mstat->nsend_enter[tid];
	  mstat->nrecv[tid] += pd.mpi.nrecv - mstat->nrecv_enter[tid];
	  mstat->ncoll[tid] += pd.mpi.ncoll - mstat->ncoll_enter[tid];
	  
	  mstat->bout[tid]  += pd.mpi.bout - mstat->bout_enter[tid];
	  mstat->bin[tid]   += pd.mpi.bin  - mstat->bin_enter[tid];
	}
#endif 
      
#ifdef OMPP_USE_PAPI
      if( tid<mstat->nctr )
	{
	  for( i=0; i<OMPP_PAPI_MAX_CTRS; i++ )
	    {
	      //fprintf(stderr, "mstat_exit i=%d tid=%d ctr=%lld\n", i, tid, pd.ctr[i]);
	      mstat->ctr[i][tid]+=(pd.ctr[i]-mstat->ctr_enter[i][mstat->rlevel[tid]][tid]);
	    }
	}
#endif
    }

}


void mstat_init( mstat_t *mstat, 
		 regstat_t *stat,
		 unsigned nexecc, unsigned nexect,
		 unsigned nctr )
{
  unsigned i, j;

  mstat->rstat=stat;

  mstat->nexect  = nexect;
  mstat->nexecc  = nexecc;
  mstat->nctr    = nctr;

  mstat->rlevel  = (unsigned*) malloc( sizeof(unsigned)*nexect );
  mstat->exect   = (double*)   malloc( sizeof(double)*nexect );
  

  for( i=0; i<OMPP_MAX_RECURSION; i++ )
    {
      mstat->exect_enter[i] = (double*) malloc( sizeof(double)*nexect );
    }

  mstat->execc       = (unsigned*) malloc( sizeof(unsigned)*nexecc );

#ifdef OMPP_MPI
  if( nctr>0 )
    {
      mstat->time  = (double*) malloc( sizeof(double)*nctr );
      mstat->nsend = (unsigned*) malloc( sizeof(unsigned)*nctr );
      mstat->nrecv = (unsigned*) malloc( sizeof(unsigned)*nctr );
      mstat->ncoll = (unsigned*) malloc( sizeof(unsigned)*nctr );
      mstat->bout  = (unsigned*) malloc( sizeof(unsigned)*nctr );
      mstat->bin   = (unsigned*) malloc( sizeof(unsigned)*nctr );

      mstat->time_enter  = (double*) malloc( sizeof(double)*nctr );
      mstat->nsend_enter = (unsigned*) malloc( sizeof(unsigned)*nctr );
      mstat->nrecv_enter = (unsigned*) malloc( sizeof(unsigned)*nctr );
      mstat->ncoll_enter = (unsigned*) malloc( sizeof(unsigned)*nctr );
      mstat->bout_enter  = (unsigned*) malloc( sizeof(unsigned)*nctr );
      mstat->bin_enter   = (unsigned*) malloc( sizeof(unsigned)*nctr );

      for( i=0; i<nctr; i++ )
	{ 
	  mstat->time[i] = 0.0;
	  mstat->nsend[i] = 0;
	  mstat->nrecv[i] = 0;
	  mstat->ncoll[i] = 0;
	  mstat->bout[i] = 0;
	  mstat->bin[i] = 0;
	}
    }
#endif /* OMPP_MPI */


#ifdef OMPP_USE_PAPI
  if( nctr>0 )
    {
      for( i=0; i<OMPP_PAPI_MAX_CTRS; i++ )
	{
	  mstat->ctr[i]       = (long long*) malloc( sizeof(long long)*nctr );
	  for( j=0; j<nctr; j++ )
	    {
	      mstat->ctr[i][j] = 0;
	    }

	  for( j=0; j<OMPP_MAX_RECURSION; j++ )
	    {
	      mstat->ctr_enter[i][j] = (long long*) malloc( sizeof(long long)*nctr );
	    }
	  

	}
    }
#endif

  for( i=0; i<nexect; i++ )
    {
      mstat->exect[i]=0.0;
      mstat->rlevel[i]=0;
    }

  for( i=0; i<nexecc; i++ )
    {
      mstat->execc[i]=0;
    }
}


void regstat_init(regstat_t *stat,
		  struct ompp_region *reg,
		  unsigned nthreads )
{
  int nctr=0;
  int n;

  assert(stat);
  assert(reg);

  stat->nthreads=nthreads;
  stat->reg=reg;

  stat->enter  = 0;
  stat->exit   = 0;
  stat->main   = 0;
  stat->main_x = 0;
  stat->body   = 0;
  stat->body_x = 0;
  stat->ibarr  = 0;
  stat->seq    = 0;
  stat->task   = 0;

#ifdef OMPP_USE_PAPI
  nctr = nthreads;
#endif 
  
#ifdef OMPP_MPI
  nctr = nthreads;
#endif
  
  switch(reg->type)
    {
      //
      // sequential region only
      // XXX: need a main_x here?
      //
    case OMPP_MASTER:
      stat->main   = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->main_x = (mstat_t*) malloc( sizeof(mstat_t) );
      mstat_init( stat->main,   stat, 1, 1, 1 );
      mstat_init( stat->main_x, stat, 1, 1, 1 );
      stat->nthreads=1;
      break;
      
      //
      // those only have the main region, and
      // nothing can be nested inside them
      //
    case OMPP_ATOMIC:
    case OMPP_FLUSH:
      stat->main = (mstat_t*) malloc( sizeof(mstat_t) );
      mstat_init( stat->main, stat, nthreads, nthreads, 0 );
      break;

      //
      // main region and account for tasks than can 
      // be executed inside
      //
    case OMPP_BARRIER:
      stat->main = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->task = (mstat_t*) malloc( sizeof(mstat_t) );
      
      mstat_init( stat->main, stat, nthreads, nthreads, 0 );
      mstat_init( stat->task, stat, nthreads, nthreads, 0 );
      break;


    case OMPP_REGION:
      stat->main   = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->main_x = (mstat_t*) malloc( sizeof(mstat_t) );

      n=ompp_concurrency;      
      mstat_init( stat->main, stat, n, n, n );
      mstat_init( stat->main_x, stat, n, n, n );
      
      /* NANOS compiler workaround      if( omp_in_parallel() )
      if(0) 
	{
	  mstat_init( stat->main, stat, nthreads, nthreads, nctr );
	  mstat_init( stat->main_x, stat, nthreads, nthreads, nctr );
	}
      else
	{
	  mstat_init( stat->main, stat, 1, 1, 1 );
	  mstat_init( stat->main_x, stat, 1, 1, 1 );
	}
      */
      break;

    
      /* XXX: does OMPP_TASK need main_x? 
	      Can nesting occur there? */
    case OMPP_TASK:
    case OMPP_TASKEXEC:
    case OMPP_UTASK:
    case OMPP_UTASKEXEC:
    case OMPP_TASKWAIT:
      stat->main   = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->main_x = (mstat_t*) malloc( sizeof(mstat_t) );
      
      mstat_init( stat->main, stat, nthreads, nthreads, nctr );
      mstat_init( stat->main_x, stat, nthreads, nthreads, nctr );
      break;

      //
      // loop has a main region and an implicit barrier
      // we're also including an artificial 'body' region 
      // to report the body time in the profiling report
      //
    case OMPP_LOOP:
    case OMPP_WORKSHARE:
      stat->main   = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->ibarr  = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->body   = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->body_x = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->task   = (mstat_t*) malloc( sizeof(mstat_t) );
      
      mstat_init( stat->main   , stat, nthreads, nthreads, 0);
      mstat_init( stat->ibarr  , stat, nthreads, nthreads, 0 );
      mstat_init( stat->body   , stat, nthreads, nthreads, nctr );
      mstat_init( stat->body_x , stat, nthreads, nthreads, nctr );
      mstat_init( stat->task   , stat, nthreads, nthreads, 0 );
      break;
      
      
      //
      // those have an additional body
      //
    case OMPP_SECTIONS:
    case OMPP_SINGLE:
      stat->main   = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->ibarr  = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->body   = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->body_x = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->task   = (mstat_t*) malloc( sizeof(mstat_t) );

      mstat_init( stat->main,   stat, nthreads, nthreads, 0 );
      mstat_init( stat->ibarr,  stat, nthreads, nthreads, 0 );
      mstat_init( stat->body,   stat, nthreads, nthreads, nctr );
      mstat_init( stat->body_x, stat, nthreads, nthreads, nctr );
      mstat_init( stat->task,   stat, nthreads, nthreads, 0 );
      break;

      //
      // critical and lock have enter and exit pseudo regions
      // nthreads will be set to 1 if ompp_collapse_locks is set
      //
    case OMPP_CRITICAL:
    case OMPP_LOCK:
      // stat->main is redundant ?!
      stat->main   = (mstat_t*) malloc( sizeof(mstat_t) ); 
      stat->body   = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->body_x = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->enter  = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->exit   = (mstat_t*) malloc( sizeof(mstat_t) );
      
      mstat_init( stat->main,   stat, nthreads, nthreads, 0 );
      mstat_init( stat->body,   stat, nthreads, nthreads, nctr );
      mstat_init( stat->body_x, stat, nthreads, nthreads, nctr );
      mstat_init( stat->enter,  stat, nthreads, nthreads, 0 );
      mstat_init( stat->exit,   stat, nthreads, nthreads, 0 );
      break;

    case OMPP_PARALLEL:
    case OMPP_PARALLEL_LOOP:
    case OMPP_PARALLEL_WORKSHARE:
      stat->enter  = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->exit   = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->seq    = (mstat_t*) malloc( sizeof(mstat_t) ); 
      stat->main   = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->body   = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->body_x = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->ibarr  = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->task   = (mstat_t*) malloc( sizeof(mstat_t) );
      
      mstat_init( stat->enter,  stat, nthreads, nthreads, 0);
      mstat_init( stat->exit,   stat, nthreads, nthreads, 0);

      mstat_init( stat->seq,    stat, 1, 1,0);
      mstat_init( stat->main,   stat, nthreads, nthreads, 0);
      mstat_init( stat->body,   stat, nthreads, nthreads, nctr);
      mstat_init( stat->body_x, stat, nthreads, nthreads, nctr);
      mstat_init( stat->ibarr,  stat, nthreads, nthreads, 0);
      mstat_init( stat->task,   stat, nthreads, nthreads, 0);

      break;
      
    case OMPP_PARALLEL_SECTIONS:
      stat->seq    = (mstat_t*) malloc( sizeof(mstat_t) ); 
      stat->enter  = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->main   = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->body   = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->body_x = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->ibarr  = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->exit   = (mstat_t*) malloc( sizeof(mstat_t) );
      stat->task   = (mstat_t*) malloc( sizeof(mstat_t) );

      mstat_init(stat->enter,  stat, nthreads, nthreads, 0);
      mstat_init(stat->exit,   stat, nthreads, nthreads, 0);
      mstat_init(stat->seq,    stat, 1, 1, 0);
      mstat_init(stat->body,   stat, nthreads, nthreads, nctr);
      mstat_init(stat->body_x, stat, nthreads, nthreads, nctr);
      mstat_init(stat->main,   stat, nthreads, nthreads, 0);
      mstat_init(stat->ibarr,  stat, nthreads, nthreads, 0);
      mstat_init(stat->task,   stat, nthreads, nthreads, 0);

      break;

    default:
      break;
    }
}


void regstat_on_enter(regstat_t *stat,
		      ompp_reg_type_t type,
		      unsigned tid, int rstack )
{
  unsigned i;
  perfdata_t pd;
  ompp_reg_type_t rtype;

#ifdef OMPP_USE_PAPI
  //  long long values[OMPP_PAPI_MAX_CTRS];
#endif

  assert(stat);
  assert(stat->reg);

  rtype = stat->reg->type;

  OMPP_TIMESTAMP( pd.time );

  switch( rtype )
    {
    case OMPP_MASTER:
      assert( stat->main );
      assert( rtype==type );

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */

#ifdef OMPP_USE_PAPI
      if( OMPP_IS_PAPI_ON )
	{
	  PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	}
#endif /* OMPP_USE_PAPI */      

      mstat_enter( stat->main, tid, pd );
      break;

    case OMPP_ATOMIC:
    case OMPP_FLUSH:
      assert( stat->main );
      assert( rtype==type );
      mstat_enter( stat->main, tid, pd );

      break;

    case OMPP_BARRIER:
      assert( stat->main );
      assert( rtype==type );
      
      mstat_enter( stat->main, tid, pd );

      if( rstack )
	ompp_task_mstat_cg[tid]=stat->task;
      else
	ompp_task_mstat[tid]=stat->task;
      
      break;
      
    case OMPP_TASKWAIT:
      assert( stat->main );
      mstat_enter( stat->main, tid, pd );
      break;


    case OMPP_REGION:
      assert( stat->main );
      assert( rtype==type );

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */

#ifdef OMPP_USE_PAPI
      if( OMPP_IS_PAPI_ON )
	{
	  PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	}
#endif /* OMPP_USE_PAPI */

      /* 
	 fprintf(stderr, "mstat_enter %d %f\n", tid, pd.time);
      */
      mstat_enter( stat->main, tid, pd );

      break;
      
    case OMPP_CRITICAL:
      assert( stat->main );
      assert( stat->enter );
      assert( stat->body );

      switch( type )
	{
	case OMPP_CRITICAL:
	  mstat_enter( stat->main, tid, pd );
	  mstat_enter( stat->enter, tid, pd );
	  break;
	case OMPP_CRITICAL_BODY:
	  mstat_exit( stat->enter, tid, pd );

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */
	  
#ifdef OMPP_USE_PAPI
	  if( OMPP_IS_PAPI_ON )
	    {
	      PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	    }
#endif /* OMPP_USE_PAPI */

	  mstat_enter( stat->body, tid, pd );
	  break;
	default:
	  assert(0);
	}
      break;
      

    case OMPP_LOOP:
    case OMPP_WORKSHARE:
      assert( stat->main );
      assert( stat->ibarr );
      assert( stat->body );

      switch( type )
	{
	case OMPP_LOOP:
	case OMPP_WORKSHARE:

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */
	  
#ifdef OMPP_USE_PAPI
	  if( OMPP_IS_PAPI_ON )
	    {
	      PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	    }
#endif /* OMPP_USE_PAPI */

	  mstat_enter( stat->main, tid, pd );
	  mstat_enter( stat->body, tid, pd );
	  break;

	case OMPP_BARRIER:

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */

#ifdef OMPP_USE_PAPI
	  if( OMPP_IS_PAPI_ON )
	    {
	      PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	    }
#endif /* OMPP_USE_PAPI */

	  mstat_exit( stat->body, tid, pd );
	  mstat_enter( stat->ibarr, tid, pd );

	  if( !rstack )
	    ompp_task_mstat[tid]=stat->task;

	  break;
	default:
	  assert(0);  
	}
      break;

    case OMPP_PARALLEL:
      assert( stat->enter );
      assert( stat->seq );
      assert( stat->main );
      assert( stat->ibarr );
      assert( stat->body );

      switch(type)
	{
	case OMPP_PARALLEL: 
	  for( i=0; i<stat->nthreads; i++ ) 
	    {
	      mstat_enter( stat->enter, i, pd );
	      mstat_enter( stat->main, i, pd );
	    }
	  mstat_enter( stat->seq, tid, pd );
	  break;
	  
	case OMPP_PARALLEL_BODY:
	  mstat_exit( stat->enter, tid, pd );

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */
	  
#ifdef OMPP_USE_PAPI
	  if( OMPP_IS_PAPI_ON )
	    {
	      PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	      //fprintf(stderr, "ENTER %d %lld\n", tid, pd.ctr[0]);
	    }
#endif /* OMPP_USE_PAPI */

	  mstat_enter( stat->body, tid, pd );
	  break;

	case OMPP_BARRIER:
	  
#ifdef OMPP_MPI
	  ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */
	  
#ifdef OMPP_USE_PAPI
	  if( OMPP_IS_PAPI_ON )
	    {
	      PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	      //fprintf(stderr, "EXIT %d %lld\n", tid, pd.ctr[0]);
	    }
#endif /* OMPP_USE_PAPI */      
	  mstat_exit( stat->body, tid, pd );
	  mstat_enter( stat->ibarr, tid, pd );

	  if( !rstack )
	    ompp_task_mstat[tid]=stat->task;

	  break;
	  
	  // this can occur since the stack 
	  // is not maintained for locks
      case OMPP_LOCK:
      case OMPP_LOCK_BODY:
	break;

	default:
	  fprintf(stderr, "type = %d %s\n", type,
		  ompp_region_full_name[type]);

	  assert(0);  
	}
      break;

    case OMPP_SECTIONS:
      assert( stat->main );
      assert( stat->body );
      assert( stat->ibarr );
      
      switch( type )
	{
	case OMPP_SECTIONS:
	  mstat_enter( stat->main, tid, pd );

	  break;

	case OMPP_SECTION:

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */

#ifdef OMPP_USE_PAPI
	  if( OMPP_IS_PAPI_ON )
	    {
	      PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	    }
#endif /* OMPP_USE_PAPI */

	  mstat_enter( stat->body, tid, pd );
	  break;

	case OMPP_BARRIER:
	  mstat_enter( stat->ibarr, tid, pd );

	  if( !rstack )
	    ompp_task_mstat[tid]=stat->task;
	  
	  break;
	default:
	  assert(0);
	}
      break;

    case OMPP_SINGLE:
      assert( stat->main );
      assert( stat->body );
      assert( stat->ibarr );

      switch( type )
	{
	case OMPP_SINGLE:
	  mstat_enter( stat->main, tid, pd );
	  break;
	  
	case OMPP_SINGLE_BODY:

#ifdef OMPP_MPI
	  ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */
	  
#ifdef OMPP_USE_PAPI
	  if( OMPP_IS_PAPI_ON )
	    {
	      PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	    }
#endif /* OMPP_USE_PAPI */
	  mstat_enter( stat->body, tid, pd );
	  break;

	case OMPP_BARRIER:
	  mstat_enter( stat->ibarr, tid, pd );

	  if( !rstack )
	    ompp_task_mstat[tid]=stat->task;
	  

	  break;
	default:
	  assert(0);
	}
      break;


    case OMPP_TASK:
    case OMPP_TASKEXEC:
    case OMPP_UTASK:
    case OMPP_UTASKEXEC:
      assert( stat->main );
      assert( rtype==type );

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */
      
#ifdef OMPP_USE_PAPI
      if( OMPP_IS_PAPI_ON )
	{
	  PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	}
#endif /* OMPP_USE_PAPI */
      
      mstat_enter( stat->main, tid, pd );
      
      // keep track in which worksharing region 
      // we executed the task in
      if( ((type==OMPP_TASKEXEC || type==OMPP_UTASKEXEC))
	  && ompp_task_mstat[tid] && !rstack )
	{
	  //fprintf(stderr, "executing task in barrier %d\n", tid);
	  if( ompp_task_nesting[tid]>0 )
	    {
	      mstat_exit( ompp_task_mstat[tid], tid, pd );
	    }
	  mstat_enter( ompp_task_mstat[tid], tid, pd );
	  //fprintf(stderr, "mstat_enter %p\n", ompp_task_mstat[tid]);

	  ompp_task_nesting[tid]++;
	}    

      
      break;

    case OMPP_PARALLEL_LOOP:
    case OMPP_PARALLEL_WORKSHARE:
	assert( stat->main );
	assert( stat->body );
	assert( stat->enter );
	assert( stat->seq );
	assert( stat->ibarr );

      switch( type )
	{
	case OMPP_PARALLEL:
	    for( i=0; i<stat->nthreads; i++ )  
	    {
		mstat_enter( stat->enter, i, pd );
		mstat_enter( stat->main, i, pd );
	    }

	  mstat_enter( stat->seq, tid, pd );
	  break;
	  
	case OMPP_PARALLEL_BODY:

	  mstat_exit( stat->enter, tid, pd );
	  break;

	case OMPP_LOOP:
	case OMPP_WORKSHARE:

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */

#ifdef OMPP_USE_PAPI
	  if( OMPP_IS_PAPI_ON )
	    {
	      PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	    }
#endif /* OMPP_USE_PAPI */

	  mstat_enter( stat->body, tid, pd );
	  break;
	  
	case OMPP_BARRIER:
	  mstat_enter( stat->ibarr, tid, pd );
	  
#ifdef OMPP_MPI
	  ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */
	  
#ifdef OMPP_USE_PAPI
	  if( OMPP_IS_PAPI_ON )
	    {
	      PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	      //fprintf(stderr, "EXIT %d %lld\n", tid, pd.ctr[0]);
	    }
#endif /* OMPP_USE_PAPI */      
	  mstat_exit( stat->body, tid, pd );

	  if( !rstack )
	    ompp_task_mstat[tid]=stat->task;	  

	  break;
	default:
	  fprintf(stderr, "*** %d\n", type);
	  assert(0);
	}
      break;

    case OMPP_LOCK:
      assert( stat->main );
      assert( stat->body );
      assert( stat->enter );
      assert( stat->exit );

      switch( type )
	{
	case OMPP_LOCK:
	  mstat_enter( stat->main, tid, pd );
	  mstat_enter( stat->enter, tid, pd );
	  break;

	case OMPP_LOCK_BODY:
	  mstat_exit( stat->enter, tid, pd );
	  mstat_enter( stat->body, tid, pd );
	  break;

	default:
	  //fprintf(stderr, "*** %d\n", type);
	  assert(0);

	  // XXX case OMPP_LOCK_FAIL:
	  //	  pregstat_on_exit( rstat->enter, tid, t );
	  //	  pregstat_on_exit( rstat->main, tid, t );
	  //	  break;
	}
      break;

    case OMPP_PARALLEL_SECTIONS:
      assert( stat->seq );
      assert( stat->enter );
      assert( stat->exit );
      assert( stat->ibarr );
      assert( stat->main );
      assert( stat->body );

      switch( type )
	{
	case OMPP_PARALLEL:
	  for( i=0; i<stat->nthreads; i++ ) 
	  {
	      mstat_enter( stat->enter, i, pd );
	      mstat_enter( stat->main, i, pd );
	  }

	  mstat_enter( stat->seq, tid, pd );
	  break;

	case OMPP_PARALLEL_BODY:

	  mstat_exit( stat->enter, tid, pd );
	  break;

	case OMPP_SECTIONS:
	    break;

	case OMPP_SECTION:

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */

#ifdef OMPP_USE_PAPI
	  if( OMPP_IS_PAPI_ON )
	    {
	      PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	    }
#endif /* OMPP_USE_PAPI */

	  mstat_enter( stat->body, tid, pd );
	  break;

	case OMPP_BARRIER:
	  mstat_enter( stat->ibarr, tid, pd );
	  
	  if( !rstack )
	    ompp_task_mstat[tid]=stat->task;

	  break;
	default:
	  assert(0);
	}
      break;
    }
}



void regstat_on_exit(regstat_t *stat,
		     ompp_reg_type_t type,
		     unsigned tid, int rstack )
{
  ompp_reg_type_t rtype;
  perfdata_t pd;
  unsigned i;
  //  long long values[1];

  static unsigned usedthreads;

  assert(stat);
  assert(stat->reg);

  rtype = stat->reg->type;

  OMPP_TIMESTAMP(pd.time);
  
  switch( rtype )
    {
    case OMPP_MASTER:
      assert( stat->main );
      assert( rtype==type );

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */

#ifdef OMPP_USE_PAPI
      if( OMPP_IS_PAPI_ON )
	{
	  PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	}
#endif /* OMPP_USE_PAPI */      

      mstat_exit( stat->main, tid, pd );
      break;
      
    case OMPP_ATOMIC:
    case OMPP_FLUSH:
      assert( stat->main );
      assert( rtype==type );
      mstat_exit( stat->main, tid, pd );
      break;

    case OMPP_BARRIER:
      assert( stat->main );
      assert( rtype==type );
      mstat_exit( stat->main, tid, pd );
      
      if( !rstack )
	ompp_task_mstat[tid]=0;

      break;

    case OMPP_TASKWAIT:
      assert( stat->main );
      mstat_exit( stat->main, tid, pd );
      break;

    case OMPP_REGION:
      assert( stat->main );
      assert( rtype==type );

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */
      
#ifdef OMPP_USE_PAPI
      if( OMPP_IS_PAPI_ON )
	{
	  PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	}
#endif /* OMPP_USE_PAPI */      

      /* fprintf(stderr, "mstat_exit %d %f\n", tid, pd.time); */

      mstat_exit( stat->main, tid, pd );
      break;

    case OMPP_CRITICAL:
      assert( stat->main );
      assert( stat->exit );
      assert( stat->body );

      switch( type )
	{
	case OMPP_CRITICAL:
	  mstat_exit( stat->main, tid, pd );
	  mstat_exit( stat->exit, tid, pd );
	  break;
	case OMPP_CRITICAL_BODY:
	  mstat_enter( stat->exit, tid, pd );

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */

#ifdef OMPP_USE_PAPI
	  if( OMPP_IS_PAPI_ON )
	    {
	      PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	    }
#endif /* OMPP_USE_PAPI */      
	  mstat_exit( stat->body, tid, pd );
	  
	  break;
	}
      break;
      
    case OMPP_LOOP:
    case OMPP_WORKSHARE:
      assert( stat->main );
      assert( stat->ibarr );
      
      switch( type )
	{
	case OMPP_LOOP:
	case OMPP_WORKSHARE:
	  mstat_exit( stat->main, tid, pd );
	  if( stat->body->rlevel[tid] > 0 )
	    {

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */

#ifdef OMPP_USE_PAPI
	      if( OMPP_IS_PAPI_ON )
		{
		  PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
		}
#endif /* OMPP_USE_PAPI */
	      mstat_exit( stat->body, tid, pd );
	    }
	  break;

	case OMPP_BARRIER:
	  mstat_exit( stat->ibarr, tid, pd );

	  if( !rstack )
	    ompp_task_mstat[tid]=0;

	  break;	  
	}
      break;

    case OMPP_PARALLEL:
      assert( stat->exit );
      assert( stat->seq );
      assert( stat->main );
      assert( stat->ibarr );
      assert( stat->body );

      switch(type)
	{
	case OMPP_PARALLEL:
	    for( i=0; i<usedthreads; i++ )
	    {
		mstat_exit( stat->exit, i, pd );
		mstat_exit( stat->main, i, pd );
	    }

	    mstat_exit( stat->seq, tid, pd );
	    break;

	case OMPP_PARALLEL_BODY:
	  mstat_enter( stat->exit, tid, pd );

	  if( tid==0 )
	      usedthreads=omp_get_num_threads();
	  break;
	  
	case OMPP_BARRIER:
	  mstat_exit( stat->ibarr, tid, pd );

	  if( !rstack )
	    ompp_task_mstat[tid]=0;

	  break;
	}
      break;

    case OMPP_SECTIONS:
      assert( stat->main );
      assert( stat->body );
      assert( stat->ibarr );
      
      switch( type )
	{
	case OMPP_SECTIONS:
	  mstat_exit( stat->main, tid, pd );

	  break;

	case OMPP_SECTION:

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */

#ifdef OMPP_USE_PAPI
	  if( OMPP_IS_PAPI_ON )
	    {
	      PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	    }
#endif /* OMPP_USE_PAPI */      

	  mstat_exit( stat->body, tid, pd );
	  break;

	case OMPP_BARRIER:
	  mstat_exit( stat->ibarr, tid, pd );
	  //	  mstat_dump( stat->ibarr, stderr );

	  if( !rstack )
	    ompp_task_mstat[tid]=0;
	  

	  break;
	}
      break;


   case OMPP_SINGLE:
      assert( stat->main );
      assert( stat->body );
      assert( stat->ibarr );

      switch( type )
	{
	case OMPP_SINGLE:
	  mstat_exit( stat->main, tid, pd );
	  break;
	  
	case OMPP_SINGLE_BODY:

#ifdef OMPP_MPI
	  ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */

#ifdef OMPP_USE_PAPI
	  if( OMPP_IS_PAPI_ON )
	    {
	      PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	    }
#endif /* OMPP_USE_PAPI */      

	  mstat_exit( stat->body, tid, pd );
	  break;

	case OMPP_BARRIER:
	  mstat_exit( stat->ibarr, tid, pd );

	  if( !rstack )
	    ompp_task_mstat[tid]=0;

	  break;
	}
      break;

    case OMPP_TASK:
    case OMPP_TASKEXEC:
    case OMPP_UTASK:
    case OMPP_UTASKEXEC:
     assert( stat->main );
     assert( rtype==type );

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */
     
#ifdef OMPP_USE_PAPI
     if( OMPP_IS_PAPI_ON )
       {
	 PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
       }
#endif /* OMPP_USE_PAPI */      
      mstat_exit( stat->main, tid, pd );

      // keep track in which worksharing region 
      // we executed the task in
      if( (rtype==OMPP_TASKEXEC || rtype==OMPP_UTASKEXEC) && ompp_task_mstat[tid] && !rstack )
	{
	  ompp_task_nesting[tid]--;

	  //fprintf(stderr, "exiting tasking... %d\n", tid);
	  mstat_exit( ompp_task_mstat[tid], tid, pd );
	  //fprintf(stderr, "mstat_exit %p\n", ompp_task_mstat[tid]);
	  if( ompp_task_nesting[tid]>0 ) {
	    mstat_enter( ompp_task_mstat[tid], tid, pd );
	  }


	}

      break;


    case OMPP_PARALLEL_LOOP:
    case OMPP_PARALLEL_WORKSHARE:
      assert( stat->exit );
      assert( stat->seq );
      assert( stat->ibarr );

      switch( type )
	{
	case OMPP_PARALLEL:
	    for( i=0; i<stat->nthreads; i++ ) 
	    {
		mstat_exit( stat->exit, i, pd );
		mstat_exit( stat->main, i, pd );
	    }
		
	    mstat_exit( stat->seq, tid, pd );

	  break;

	case OMPP_PARALLEL_BODY:
	  mstat_enter( stat->exit, tid, pd );
	  break;

	case OMPP_LOOP:
	  break;
	  
	case OMPP_BARRIER:
	  mstat_exit( stat->ibarr, tid, pd );

	  if( !rstack )
	    ompp_task_mstat[tid]=0;

	  break;
	}
      break;

    case OMPP_LOCK:
      assert( stat->main );
      assert( stat->body );
      assert( stat->enter );
      assert( stat->exit );

      switch( type )
	{
	case OMPP_LOCK:
	  mstat_exit( stat->exit, tid, pd );
	  mstat_exit( stat->main, tid, pd );
	  break;

	case OMPP_LOCK_BODY:
	  mstat_exit( stat->body, tid, pd );
	  mstat_enter( stat->exit, tid, pd );
	  break;

	case OMPP_LOCK_FAIL:
	  mstat_exit( stat->enter, tid, pd );
	  mstat_exit( stat->main, tid, pd );
	  break;
	}
      break;

    case OMPP_PARALLEL_SECTIONS:
      assert( stat->seq );
      assert( stat->enter );
      assert( stat->exit );
      assert( stat->ibarr );
      assert( stat->main );
      assert( stat->body );

      switch( type )
	{
	case OMPP_PARALLEL:
	  for( i=0; i<stat->nthreads; i++ ) 
	  {
	    mstat_exit( stat->exit, i, pd );
	    mstat_exit( stat->main, i, pd );
	  }

	  mstat_exit( stat->seq, tid, pd );
	  break;

	case OMPP_PARALLEL_BODY:
	  mstat_enter( stat->exit, tid, pd );
	  break;

	case OMPP_BARRIER:
	  mstat_exit( stat->ibarr, tid, pd );

	  if( !rstack )
	    ompp_task_mstat[tid]=0;

	  break;

	case OMPP_SECTION:

#ifdef OMPP_MPI
      ompp_mpi_read( &(pd.mpi), tid );
#endif /* OMPP_MPI */

#ifdef OMPP_USE_PAPI
	    if( OMPP_IS_PAPI_ON )
	    {
		PAPI_read(ompp_papi_ctr_evtset[tid], pd.ctr);
	    }
#endif /* OMPP_USE_PAPI */
	    
	  mstat_exit( stat->body, tid, pd );
	  break;
	  
	case OMPP_SECTIONS:
	  break;
	}
      break;
    }
}


void mstat_dump( mstat_t *mstat, FILE *f )
{
  unsigned i;
  for( i=0; i<mstat->nexect; i++ )
    {
      fprintf(f, "%f (%u) ", mstat->exect[i], mstat->execc[i]);
    }
  fprintf(f, "\n");
}

void mstat_dump_ctr( mstat_t *mstat, FILE *f )
{
#ifdef OMPP_USE_PAPI
  unsigned i;
  for( i=0; i<mstat->nctr; i++ )
    {
      fprintf(f, "%lld ", mstat->ctr[0][i] );
    }
  fprintf(f, "\n");
#endif
}


void pline_clear( pline_t *pd )
{  
  int i;
  
  pd->exect=0.0;
  pd->exect_x=0.0;
  pd->execc=0;
  pd->ibarrt=0.0;
  pd->bodyt=0.0;
  pd->bodyt_x=0.0;
  pd->bodyc=0;
  pd->entert=0.0;
  pd->enterc=0;
  pd->exitt=0.0;
  pd->exitc=0;
  pd->taskt=0.0;

#ifdef OMPP_MPI
  pd->mpi.time=0.0;
  pd->mpi.nsend=0;
  pd->mpi.nrecv=0;
  pd->mpi.ncoll=0;
  pd->mpi.bout=0;
  pd->mpi.bin=0;
#endif /* OMPP_MPI */  

#ifdef OMPP_USE_PAPI
  for( i=0; i<OMPP_PAPI_MAX_CTRS; i++ )
    {
      pd->ctr[i]=0;
      pd->ctr_x[i]=0;
    }
#endif
}

// clear and reset selector
void pline_init( pline_t *pd )
{
  pline_clear(pd);
  pd->selector=0;
}

void pline_addto( pline_t *pd1, pline_t *pd2 )
{
  int i;

  pd1->exect+=pd2->exect;
  pd1->exect_x+=pd2->exect_x;
  pd1->execc+=pd2->execc;
  pd1->ibarrt+=pd2->ibarrt;
  pd1->bodyt+=pd2->bodyt;
  pd1->bodyt_x+=pd2->bodyt_x;
  pd1->bodyc+=pd2->bodyc;
  pd1->entert+=pd2->entert;
  pd1->enterc+=pd2->enterc;
  pd1->exitt+=pd2->exitt;
  pd1->exitc+=pd2->exitc;
  pd1->taskt+=pd2->taskt;

#ifdef OMPP_MPI
  pd1->mpi.time  += pd2->mpi.time;
  pd1->mpi.nsend += pd2->mpi.nsend;
  pd1->mpi.nrecv += pd2->mpi.nrecv;
  pd1->mpi.ncoll += pd2->mpi.ncoll;
  pd1->mpi.bout += pd2->mpi.bout; 
  pd1->mpi.bin += pd2->mpi.bin;
#endif 

#ifdef OMPP_USE_PAPI
  for( i=0; i<OMPP_PAPI_MAX_CTRS; i++ )
    {
      pd1->ctr[i]+=pd2->ctr[i];
      pd1->ctr_x[i]+=pd2->ctr_x[i];
    }
#endif
}


void pline_print( FILE* f, int tid, pline_t *pd )
{
  int i, j;
  char buf[40];
  double res;

#ifdef OMPP_USE_PAPI
  /* counter values converted to doubles */
  double dctr[OMPP_PAPI_MAX_CTRS+2];
#endif

  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      if( tid>=0 )
	fprintf(f, " %3d", tid);
      else
	fprintf(f, " SUM");
      
      if( (pd->selector & PD_EXECT) )
	fprintf(f, " %10.2f", pd->exect );
      
      if( (pd->selector & PD_EXECT_X) )
	fprintf(f, " %10.2f", pd->exect_x );
      
      if( (pd->selector & PD_EXECC) )
	fprintf(f, " %10u", pd->execc );

      if( (pd->selector & PD_BODYT) )
	fprintf(f, " %10.2f", pd->bodyt);

      if( (pd->selector & PD_BODYT_X) )
	fprintf(f, " %10.2f", pd->bodyt_x);

      if( (pd->selector & PD_BODYC) ) 
	fprintf(f, " %10u", pd->bodyc);
      
      if( (pd->selector & PD_IBARRT) ) {
	if( (pd->selector & PD_TASKT) ) {
	  fprintf(f, " %10.2f", pd->ibarrt-pd->taskt);
	} else {
	  fprintf(f, " %10.2f", pd->ibarrt);
	}
      }
	
      if( (pd->selector & PD_ENTERT) )
	fprintf(f, " %10.2f", pd->entert);
      
      if( (pd->selector & PD_ENTERC) )
	fprintf(f, " %10u", pd->enterc);
      
      if( (pd->selector & PD_EXITT) )
	fprintf(f, " %10.2f", pd->exitt);
      
      if( (pd->selector & PD_EXITC) )
	fprintf(f, " %10u", pd->exitc);

      if( (pd->selector & PD_MGMTT) )
	fprintf(f, " %10.2f", 
		((pd->exect)-(pd->bodyt+pd->ibarrt)));

      if( (pd->selector & PD_TASKT) )
	fprintf(f, " %10.2f", pd->taskt);


#ifdef OMPP_MPI
      if( (pd->selector & PD_MPI_TIME) )
	fprintf(f, " %10.2f", pd->mpi.time );
      if( (pd->selector & PD_MPI_BIN) )
	fprintf(f, " %10u", pd->mpi.bin );
      if( (pd->selector & PD_MPI_BOUT) )
	fprintf(f, " %10u", pd->mpi.bout );
      if( (pd->selector & PD_MPI_NRECV) )
	fprintf(f, " %10u", pd->mpi.nrecv );
      if( (pd->selector & PD_MPI_NSEND) )
	fprintf(f, " %10u", pd->mpi.nsend );
      if( (pd->selector & PD_MPI_NCOLL) )
	fprintf(f, " %10u", pd->mpi.ncoll );
#endif /* OMPP_MPI */

      break;

    case OMPP_OUTFORMAT_CSV:
        if( tid>=0 )
	  fprintf(f, "%d", tid);
	else
	  fprintf(f, "SUM");
	
	if( (pd->selector & PD_EXECT) )
	  fprintf(f, "%c%f", ompp_sep, pd->exect );

	if( (pd->selector & PD_EXECT_X) )
	  fprintf(f, "%c%f", ompp_sep, pd->exect_x );

	if( (pd->selector & PD_EXECC) )
	  fprintf(f, "%c%u", ompp_sep, pd->execc );
	
	if( (pd->selector & PD_BODYT) )
	  fprintf(f, "%c%f", ompp_sep, pd->bodyt);

	if( (pd->selector & PD_BODYT_X) )
	  fprintf(f, "%c%f", ompp_sep, pd->bodyt_x);
	
	if( (pd->selector & PD_BODYC) )
	  fprintf(f, "%c%u", ompp_sep, pd->bodyc);

	if( (pd->selector & PD_IBARRT) ) {
	  if( (pd->selector & PD_TASKT) ) {
	    fprintf(f, "%c%f", ompp_sep, pd->ibarrt-pd->taskt);
	  } else {
	    fprintf(f, "%c%f", ompp_sep, pd->ibarrt);
	  }
	}	

	if( (pd->selector & PD_ENTERT) )
	  fprintf(f, "%c%f", ompp_sep, pd->entert);
	
	if( (pd->selector & PD_ENTERC) )
	  fprintf(f, "%c%u", ompp_sep, pd->enterc);
	
	if( (pd->selector & PD_EXITT) )
	  fprintf(f, "%c%f", ompp_sep, pd->exitt);
	
	if( (pd->selector & PD_EXITC) )
	  fprintf(f, "%c%u", ompp_sep, pd->exitc);
	
	if( (pd->selector & PD_MGMTT) )
	  fprintf(f, "%c%f", ompp_sep, 
		  ((pd->exect)-(pd->bodyt+pd->ibarrt)));

	if( (pd->selector & PD_TASKT) )
	  fprintf(f, "%c%f", ompp_sep, pd->taskt);
	

#ifdef OMPP_MPI
	if( (pd->selector & PD_MPI_TIME) )
	  fprintf(f, "%c%f", ompp_sep, pd->mpi.time );
	
	if( (pd->selector & PD_MPI_BIN) )
	  fprintf(f, "%c%u", ompp_sep, pd->mpi.bin );
	
	if( (pd->selector & PD_MPI_BOUT) )
	  fprintf(f, "%c%u", ompp_sep, pd->mpi.bout );
	
	if( (pd->selector & PD_MPI_NRECV) )
	  fprintf(f, "%c%u", ompp_sep, pd->mpi.nrecv );
	
	if( (pd->selector & PD_MPI_NSEND) )
	  fprintf(f, "%c%u", ompp_sep, pd->mpi.nsend );
	
	if( (pd->selector & PD_MPI_NCOLL) )
	  fprintf(f, "%c%u", ompp_sep, pd->mpi.ncoll );
#endif

	break;
    }
  
  
#ifdef OMPP_USE_PAPI
  if( ((pd->selector&PD_CTR_B) ||  (pd->selector&PD_CTR_M)) )
    {
      for( i=0, j=0; i<OMPP_PAPI_MAX_CTRS; i++ )
	{
	  if( ompp_papi_ctr_name[i] )
	    {
	      // ctr is enabled!
	      
	      if( ((pd->selector&PD_CTR_B_X) ||  (pd->selector&PD_CTR_M_X)) )
		{
		  // print with inclusive/exclusive date
		  switch( ompp_outformat )
		    {
		    case OMPP_OUTFORMAT_PLAIN:
		      lldtostr( buf, pd->ctr[j] );
		      fprintf(f, " %20s", buf);
		      lldtostr( buf, pd->ctr_x[j] );
		      fprintf(f, " %20s", buf);
		      break;
		    case OMPP_OUTFORMAT_CSV:
		      fprintf(f, "%c%lld", ompp_sep, pd->ctr[j]);
		      fprintf(f, "%c%lld", ompp_sep, pd->ctr_x[j]);
		      break;
		    }
		}
	      else
		{
		  switch( ompp_outformat )
		    {
		    case OMPP_OUTFORMAT_PLAIN:
		      lldtostr( buf, pd->ctr[j] );
		      fprintf(f, " %18s", buf);
		      break;
		    case OMPP_OUTFORMAT_CSV:
		      fprintf(f, "%c%lld", ompp_sep, pd->ctr[j]);
		      break;
		    }
		}

	      j++;
	    }
	}
        
      for( i=0; i<OMPP_MAX_EVALUATORS; i++ )
	{
	  for( j=0; j<OMPP_PAPI_MAX_CTRS; j++ )
	    {
	      dctr[j] = ((double)pd->ctr[j]);
	    }

	  if( (pd->selector&PD_CTR_B) )
	    {
	      dctr[OMPP_PAPI_MAX_CTRS]   = (double) (pd->bodyt);
	      dctr[OMPP_PAPI_MAX_CTRS+1] = (double) (pd->bodyc);
	    }
	  if( (pd->selector&PD_CTR_M) )
	    {
	      dctr[OMPP_PAPI_MAX_CTRS]   = (double) (pd->exect);
	      dctr[OMPP_PAPI_MAX_CTRS+1] = (double) (pd->execc);
	    }


	  
	  if( ompp_eval_string[i] )
	    {
	      res = evaluator_evaluate( ompp_eval_evaluator[i],
					OMPP_PAPI_MAX_CTRS+2,
					ompp_papi_ctr_eval_name,
					dctr );
	      switch( ompp_outformat )
		{
		case OMPP_OUTFORMAT_PLAIN:
		  fprintf(f, " %12.2f ", res  );
		  break;
		case OMPP_OUTFORMAT_CSV:
		  fprintf(f, "%c%f", ompp_sep, res );
		  break;
		}
	    }
	}
    }
#endif 
  
  fprintf(f, "\n");
}



void regstat_print_header( regstat_t *stat, FILE *f )
{
  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      if( stat->reg->type==OMPP_LOCK )
	{
	  // the 'location' string already contains LOCK for 
	  // locks, so we don't need to repeat it here
	  fprintf(f, "R%05u %s\n", stat->reg->number,
		  stat->reg->location );
	}
      else
	{
	  fprintf(f, "R%05u %s %s\n", stat->reg->number,
		  stat->reg->location,
		  ompp_region_full_name[stat->reg->type] );
	}
      break;
      
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "R%05u%c%s%c%s\n", 
	      stat->reg->number, ompp_sep, 
	      ompp_region_full_name[stat->reg->type], ompp_sep,
	      stat->reg->csvloc );

      break;
    }

}


void regstat_print( FILE *f, regstat_t *stat, int excl )
{
  int i, j;
  char buf[40];

  struct pline pd, pd_sum;
  pd.selector=0;

  assert(f);
  assert(stat); 
  assert(stat->reg);
  
  pline_init( &pd );
  pline_init( &pd_sum );


  switch( stat->reg->type )
    {
    case OMPP_MASTER:

      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  if( excl )
	    fprintf(f, " TID %10s %10s %10s", 
		    "execT/I", "execT/E", "execC");
	  else
	    fprintf(f, " TID %10s %10s", 
		    "execT", "execC");
	  break;
	  
	case OMPP_OUTFORMAT_CSV:
	  if( excl )
	    fprintf(f, "TID%c%s%c%s%c%s", 
		    ompp_sep, "execT/I", 
		    ompp_sep, "execT/E", 
		    ompp_sep, "execC");
	  else
	    fprintf(f, "TID%c%s%c%s", 
		    ompp_sep, "execT", 
		    ompp_sep, "execC");
	  break;
	}

      pd.selector = PD_EXECT | PD_EXECC | (excl?PD_EXECT_X:0);

#ifdef OMPP_USE_PAPI
      pd.selector |= PD_CTR_M;
      pd.selector |= (excl?PD_CTR_M_X:0);
#endif
      break;

      //
      // those do not have nested sub-regions
      // and no counters
      //
    case OMPP_ATOMIC:
    case OMPP_FLUSH:
      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  fprintf(f, " TID %10s %10s",
		  "execT", "execC");
	  break;
	  
	case OMPP_OUTFORMAT_CSV:
	  fprintf(f, "TID%c%s%c%s",
		  ompp_sep, "execT", 
		  ompp_sep, "execC");
	  break;
	}
      
      pd.selector = PD_EXECT | PD_EXECC;

      break;


    case OMPP_BARRIER:
      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  fprintf(f, " TID %10s %10s %10s",
		  "execT", "execC", "taskT");
	  break;
	  
	case OMPP_OUTFORMAT_CSV:
	  fprintf(f, "TID%c%s%c%s%c%s",
		  ompp_sep, "execT", 
		  ompp_sep, "execC",
		  ompp_sep, "taskT" );
	  break;
	}
      
      pd.selector = PD_EXECT | PD_EXECC | PD_TASKT;
      break;

      
    case OMPP_REGION:
      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  if( excl )
	    fprintf(f, " TID %10s %10s %10s",
		    "execT/I", "execT/E", "execC");
	  else
	    fprintf(f, " TID %10s %10s",
		    "execT", "execC");
	  break;
	case OMPP_OUTFORMAT_CSV:
	  if( excl )
	    fprintf(f, "TID%c%s%c%s%c%s",
		    ompp_sep, "execT/I",
		    ompp_sep, "execT/E",
		    ompp_sep, "execC");
	  else
	    fprintf(f, "TID%c%s%c%s",
		    ompp_sep, "execT",
		    ompp_sep, "execC");
	  break;
	}

      pd.selector = PD_EXECT | PD_EXECC | (excl?PD_EXECT_X:0);
      
#ifdef OMPP_USE_PAPI
      pd.selector |= PD_CTR_M;
      pd.selector |= (excl?PD_CTR_M_X:0);
#endif       
      break;

    case OMPP_LOCK:

      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  fprintf(f, " TID %10s %10s %10s %10s %10s", 
		  "execT", "execC", "bodyT", "enterT", "exitT");
	  break;

	case OMPP_OUTFORMAT_CSV:
	  fprintf(f, "TID%c%s%c%s%c%s%c%s%c%s", 
		  ompp_sep, "execT", 
		  ompp_sep, "execC", 
		  ompp_sep, "bodyT", 
		  ompp_sep, "enterT", 
		  ompp_sep, "exitT");
	  break;
	}

      pd.selector = PD_EXECT | PD_EXECC | PD_BODYT | PD_ENTERT | PD_EXITT;
      break;

    case OMPP_CRITICAL:

      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  if( excl )
	    fprintf(f, " TID %10s %10s %10s %10s %10s %10s", 
		    "execT", "execC", "bodyT/I", "bodyT/E", "enterT", "exitT");
	  else
	    fprintf(f, " TID %10s %10s %10s %10s %10s", 
		    "execT", "execC", "bodyT", "enterT", "exitT");
	  break;
	  
	case OMPP_OUTFORMAT_CSV:
	  if( excl )
	    fprintf(f, "TID%c%s%c%s%c%s%c%s%c%s%c%s", 
		    ompp_sep, "execT",
		    ompp_sep, "execC",
		    ompp_sep, "bodyT/I",
		    ompp_sep, "bodyT/E",
		    ompp_sep, "enterT",
		    ompp_sep, "exitT");
	  else
	    fprintf(f, "TID%c%s%c%s%c%s%c%s%c%s", 
		    ompp_sep, "execT",
		    ompp_sep, "execC",
		    ompp_sep, "bodyT",
		    ompp_sep, "enterT",
		    ompp_sep, "exitT");
	  break;
	}
      
      pd.selector = PD_EXECT | PD_EXECC | PD_BODYT 
	| PD_ENTERT | PD_EXITT | (excl?PD_BODYT_X:0);
      
#ifdef OMPP_USE_PAPI
      pd.selector |= PD_CTR_B;
      pd.selector |= (excl?PD_CTR_B_X:0);
#endif 
      break;
      
    case OMPP_LOOP:
    case OMPP_WORKSHARE:

      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  if( excl )
	    fprintf(f, " TID %10s %10s %10s %10s %10s %10s", 
		    "execT", "execC", "bodyT/I", "bodyT/E", "exitBarT", "taskT");
	  else
	    fprintf(f, " TID %10s %10s %10s %10s %10s", 
		    "execT", "execC", "bodyT", "exitBarT", "taskT");
	  break;
	  
	case OMPP_OUTFORMAT_CSV:
	  if( excl )
	    fprintf(f, "TID%c%s%c%s%c%s%c%s%c%s%c%s", 
		    ompp_sep, "execT",
		    ompp_sep, "execC",
		    ompp_sep, "bodyT/I",
		    ompp_sep, "bodyT/E",
		    ompp_sep, "exitBarT",
		    ompp_sep, "taskT" );
	  else
	    fprintf(f, "TID%c%s%c%s%c%s%c%s%c%s",
		    ompp_sep, "execT",
		    ompp_sep, "execC",
		    ompp_sep, "bodyT",
		    ompp_sep, "exitBarT",
		    ompp_sep, "taskT" );
	  break;
	}

      pd.selector = PD_EXECT | PD_EXECC | PD_IBARRT | 
	PD_BODYT | (excl?PD_BODYT_X:0) | PD_TASKT ;
      
#ifdef OMPP_USE_PAPI
      pd.selector |= PD_CTR_B;
      pd.selector |= (excl?PD_CTR_B_X:0);
#endif 
      break;

    case OMPP_PARALLEL:
    case OMPP_PARALLEL_LOOP:
    case OMPP_PARALLEL_WORKSHARE:
      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  if( excl ) 
	    fprintf(f, " TID %10s %10s %10s %10s %10s %10s %10s %10s", 
		    "execT", "execC", "bodyT/I", "bodyT/E", "exitBarT", "startupT", "shutdwnT", "taskT" );
	  else
	    fprintf(f, " TID %10s %10s %10s %10s %10s %10s %10s", 
		    "execT", "execC", "bodyT", "exitBarT", "startupT", "shutdwnT", "taskT" );
	  break;
	  
	case OMPP_OUTFORMAT_CSV:
	  if( excl ) 
	    fprintf(f, "TID%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s", 
		    ompp_sep, "execT",
		    ompp_sep, "execC",
		    ompp_sep, "bodyT/I",
		    ompp_sep, "bodyT/E",
		    ompp_sep, "exitBarT",
		    ompp_sep, "startupT",
		    ompp_sep, "shutdwnT",
		    ompp_sep, "taskT" );
	  else
	    fprintf(f, "TID%c%s%c%s%c%s%c%s%c%s%c%s%c%s", 
		    ompp_sep, "execT",
		    ompp_sep, "execC",
		    ompp_sep, "bodyT",
		    ompp_sep, "exitBarT",
		    ompp_sep, "startupT",
		    ompp_sep, "shutdwnT",
		    ompp_sep, "taskT" );
	  break;
	}
      
      pd.selector = PD_EXECT | PD_EXECC | PD_BODYT | PD_IBARRT | 
	PD_ENTERT  | PD_EXITT | (excl?PD_BODYT_X:0) | PD_TASKT;
      
#ifdef OMPP_USE_PAPI 
      pd.selector |= PD_CTR_B;
      pd.selector |= (excl?PD_CTR_B_X:0);
#endif 
      break;
      
    case OMPP_SECTIONS:
      
      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  if( excl )
	    fprintf(f, " TID %10s %10s %10s %10s %10s %10s %10s %10s", 
		    "execT", "execC", "sectT/I", "sectT/E", 
		    "sectC", "exitBarT", "mgmtT", "taskT" );
	  else
	    fprintf(f, " TID %10s %10s %10s %10s %10s %10s %10s", 
		    "execT", "execC", "sectT", 
		    "sectC", "exitBarT", "mgmtT", "taskT" );
	  break;

	case OMPP_OUTFORMAT_CSV:
	  if( excl )
	    fprintf(f, "TID%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s", 
		    ompp_sep, "execT",
		    ompp_sep, "execC",
		    ompp_sep, "sectT/I",
		    ompp_sep, "sectT/E", 
		    ompp_sep, "sectC",
		    ompp_sep, "exitBarT",
		    ompp_sep, "mgmtT",
		    ompp_sep, "taskT" );
	  else
	    fprintf(f, "TID%c%s%c%s%c%s%c%s%c%s%c%s%c%s", 
		    ompp_sep, "execT",
		    ompp_sep, "execC",
		    ompp_sep, "sectT", 
		    ompp_sep, "sectC",
		    ompp_sep, "exitBarT",
		    ompp_sep, "mgmtT",
		    ompp_sep, "taskT" );
	  break;
	}

      pd.selector = PD_EXECT | PD_EXECC | PD_IBARRT | 
	PD_BODYT  | PD_BODYC | (excl?PD_BODYT_X:0) | PD_TASKT ;

      pd.selector |= PD_MGMTT;

#ifdef OMPP_USE_PAPI
      pd.selector |= PD_CTR_B;
      pd.selector |= (excl?PD_CTR_B_X:0);
#endif 
      break;

    case OMPP_PARALLEL_SECTIONS:

      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  if( excl )
	    fprintf(f, " TID %10s %10s %10s %10s %10s %10s %10s %10s %10s", 
		    "execT", "execC", "sectT/I", "sectT/E", "sectC", 
		    "exitBarT", "startupT", "shutdwnT", "taskT" );
	  else
	    fprintf(f, " TID %10s %10s %10s %10s %10s %10s %10s %10s", 
		    "execT", "execC", "sectT", "sectC",
		    "exitBarT", "startupT", "shutdwnT", "taskT" );
	  break;

	case OMPP_OUTFORMAT_CSV:
	  if( excl )
	    fprintf(f, "TID%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s", 
		    ompp_sep, "execT",
		    ompp_sep, "execC",
		    ompp_sep, "sectT/I",
		    ompp_sep, "sectT/E",
		    ompp_sep, "sectC", 
		    ompp_sep, "exitBarT",
		    ompp_sep, "startupT",
		    ompp_sep, "shutdwnT",
		    ompp_sep, "taskT" );
	  else
	    fprintf(f, "TID%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s", 
		    ompp_sep, "execT",
		    ompp_sep, "execC",
		    ompp_sep, "sectT",
		    ompp_sep, "sectC", 
		    ompp_sep, "exitBarT",
		    ompp_sep, "startupT",
		    ompp_sep, "shutdwnT",
		    ompp_sep, "taskT" );
	  break;
	}
      
      pd.selector = PD_EXECT | PD_EXECC | PD_IBARRT | 
	PD_BODYT | PD_BODYC | PD_ENTERT  | PD_EXITT | (excl?PD_BODYT_X:0) | PD_TASKT;

#ifdef OMPP_USE_PAPI
      pd.selector |= PD_CTR_B;
      pd.selector |= (excl?PD_CTR_B_X:0);
#endif 
      break;
      
    case OMPP_SINGLE:

      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  if( excl )
	    fprintf(f, " TID %10s %10s %10s %10s %10s %10s %10s", 
		    "execT", "execC", "singleT/I", "singleT/E", "singleC", "exitBarT", "taskT" );
	  else
	    fprintf(f, " TID %10s %10s %10s %10s %10s %10s", 
		    "execT", "execC", "singleT", "singleC", "exitBarT", "taskT" );
	  break;

	case OMPP_OUTFORMAT_CSV:
	  if( excl )
	    fprintf(f, "TID%c%s%c%s%c%s%c%s%c%s%c%s%c%s",
		    ompp_sep, "execT",
		    ompp_sep, "execC",
		    ompp_sep, "singleT/I",
		    ompp_sep, "singleT/E",
		    ompp_sep, "singleC",
		    ompp_sep, "exitBarT",
		    ompp_sep, "taskT" );
	  else
	    fprintf(f, "TID%c%s%c%s%c%s%c%s%c%s%c%s", 
		    ompp_sep, "execT",
		    ompp_sep, "execC",
		    ompp_sep, "singleT",
		    ompp_sep, "singleC",
		    ompp_sep, "exitBarT", 
		    ompp_sep, "taskT" );
	  break;
	}
      
      pd.selector = PD_EXECT | PD_EXECC | PD_IBARRT | 
	PD_BODYT  | PD_BODYC | (excl?PD_BODYT_X:0) | PD_TASKT ;

#ifdef OMPP_USE_PAPI
      pd.selector |= PD_CTR_B;
      pd.selector |= (excl?PD_CTR_B_X:0);
#endif 
      break;
      
    case OMPP_TASK:
    case OMPP_TASKEXEC:
    case OMPP_UTASK:
    case OMPP_UTASKEXEC:
    case OMPP_TASKWAIT:
      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  if( excl )
	    fprintf(f, " TID %10s %10s %10s",
		    "execT/I", "execT/E", "execC");
	  else
	    fprintf(f, " TID %10s %10s",
		    "execT", "execC");
	  break;
	case OMPP_OUTFORMAT_CSV:
	  if( excl )
	    fprintf(f, "TID%c%s%c%s%c%s",
		    ompp_sep, "execT/I",
		    ompp_sep, "execT/E",
		    ompp_sep, "execC");
	  else
	    fprintf(f, "TID%c%s%c%s",
		    ompp_sep, "execT",
		    ompp_sep, "execC");
	  break;
	}

      pd.selector = PD_EXECT | PD_EXECC | (excl?PD_EXECT_X:0);
      
#ifdef OMPP_USE_PAPI
      pd.selector |= PD_CTR_M;
      pd.selector |= (excl?PD_CTR_M_X:0);
#endif       
      

      break;
    }


  // print header for the counters
  
#ifdef OMPP_USE_PAPI
  if(  (pd.selector&PD_CTR_B) ||  (pd.selector&PD_CTR_M) )
    {
      for( j=0; j<OMPP_PAPI_MAX_CTRS; j++ )
	{
	  if( ompp_papi_ctr_name[j] )
	    {
	      if( excl )
		{
		  switch( ompp_outformat )
		    {
		    case OMPP_OUTFORMAT_PLAIN:
		      fprintf(f, " %18s/I", ompp_papi_ctr_name[j]);
		      fprintf(f, " %18s/E", ompp_papi_ctr_name[j]);
		      break;
		    case OMPP_OUTFORMAT_CSV:
		      fprintf(f, "%c%s/I", 
			      ompp_sep, ompp_papi_ctr_name[j]);
		      fprintf(f, "%c%s/E", 
			      ompp_sep, ompp_papi_ctr_name[j]);
		      break;
		    }
		}
	      else
		{
		  switch( ompp_outformat )
		    {
		    case OMPP_OUTFORMAT_PLAIN:
		      fprintf(f, " %18s", ompp_papi_ctr_name[j]);
		      break;
		    case OMPP_OUTFORMAT_CSV:
		      fprintf(f, "%c%s", 
			      ompp_sep, ompp_papi_ctr_name[j]);
		      break;
		    }
		}
	    }
	}

      for( j=0; j<OMPP_MAX_EVALUATORS; j++ )
	{
	  if( ompp_eval_string[j] )
	    {
	      sprintf(buf, "OMPP_EVAL%-2d", j+1);

	      switch( ompp_outformat )
		{
		case OMPP_OUTFORMAT_PLAIN:
		  fprintf(f, " %13s", buf);
		  break;
		case OMPP_OUTFORMAT_CSV:
		  fprintf(f, "%c%s", ompp_sep, buf);
		  break;
		}
	    }
	}
 
    }
#endif

#ifdef OMPP_MPI
  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, " %10s %10s %10s %10s %10s %10s",
	      "mpiT", "inV", "outV", "recvC", "sendC", "collC");
      break;
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "%c%s%c%s%c%s%c%s%c%s%c%s",
	      ompp_sep, "mpiT", 
	      ompp_sep, "inV",
	      ompp_sep, "outV",
	      ompp_sep, "recvC",
	      ompp_sep, "sendC",
	      ompp_sep, "collC");
      break;
    }
  
  pd.selector |= PD_MPI_TIME | PD_MPI_NSEND | 
    PD_MPI_NRECV | PD_MPI_NCOLL | PD_MPI_BOUT | PD_MPI_BIN;
#endif

  fprintf(f, "\n");


  pd_sum.selector=pd.selector;

  for( i=0; i<stat->nthreads; i++ )
    {
      pline_clear( &pd );

      if( (pd.selector&PD_EXECT) && stat->main && (i < stat->main->nexect) )
	pd.exect = stat->main->exect[i];

      if( (pd.selector&PD_EXECT) && stat->main_x && (i < stat->main->nexect) )
	pd.exect_x = stat->main_x->exect[i];

      if( stat->main && (i < stat->main->nexecc) )
	pd.execc = stat->main->execc[i];

      if( stat->seq && (i <stat->seq->nexect) )
	pd.exect = stat->seq->exect[i];

      if( stat->seq && (i <stat->seq->nexecc) )
	pd.execc = stat->seq->execc[i];

      if( stat->ibarr && (i < stat->ibarr->nexect ) )
	pd.ibarrt = stat->ibarr->exect[i];

      if( stat->body && (i < stat->body->nexect) )
	pd.bodyt = stat->body->exect[i];

      if( stat->body_x && (i < stat->body->nexect) )
	pd.bodyt_x = stat->body_x->exect[i];

      if( stat->body && (i < stat->body->nexecc) )
	pd.bodyc = stat->body->execc[i];

      if( stat->enter && (i < stat->enter->nexect) )
	pd.entert = stat->enter->exect[i];

      if( stat->enter && (i < stat->enter->nexecc) )
	pd.enterc = stat->enter->execc[i];

      if( stat->exit && (i < stat->exit->nexect) )
	pd.exitt = stat->exit->exect[i];

      if( stat->exit && (i < stat->exit->nexecc) )
	pd.exitc = stat->exit->execc[i];

      if( stat->task && (i < stat->task->nexect) )
	pd.taskt = stat->task->exect[i];

#ifdef OMPP_MPI
      {	
	mstat_t *mstat=0;
	
	if( stat->main && (i<stat->main->nctr) )
	  mstat = stat->main;

	if( stat->body && (i<stat->body->nctr) )
	  mstat = stat->body;

	if( mstat && pd.selector & PD_MPI_TIME )
	  pd.mpi.time = mstat->time[i];
	
	if( mstat && pd.selector & PD_MPI_NSEND )
	  pd.mpi.nsend = mstat->nsend[i];
	
	if( mstat && pd.selector & PD_MPI_NRECV )
	  pd.mpi.nrecv = mstat->nrecv[i];

	if( mstat && pd.selector & PD_MPI_NCOLL )
	  pd.mpi.ncoll = mstat->ncoll[i];

	if( mstat && pd.selector & PD_MPI_BOUT )
	  pd.mpi.bout = mstat->bout[i];
	
	if( mstat && pd.selector & PD_MPI_BIN )
	  pd.mpi.bin = mstat->bin[i];
      }

#endif

#ifdef OMPP_USE_PAPI
      if( (pd.selector&PD_CTR_M) && stat->main && (i<stat->main->nctr) )
	{
	  for( j=0; j<OMPP_PAPI_MAX_CTRS; j++ )
	    {
	      pd.ctr[j]   = stat->main->ctr[j][i];
	      pd.ctr_x[j] = stat->main_x->ctr[j][i];
	    }
	}

      if( (pd.selector&PD_CTR_B) && stat->body && (i<stat->body->nctr) )
	{
	  for( j=0; j<OMPP_PAPI_MAX_CTRS; j++ )
	    {
	      pd.ctr[j]   = stat->body->ctr[j][i];
	      pd.ctr_x[j] = stat->body_x->ctr[j][i];
	    }
	}
#endif

      pline_print(f, i, &pd);
      pline_addto(&pd_sum, &pd);
    }
  pline_print(f, -1, &pd_sum);

}



void regstat_dump( regstat_t *stat, FILE *f )
{
  fprintf(f, " Printing statistics for %s\n", ompp_region_name[stat->reg->type]);

  if( stat->reg->type == OMPP_LOCK )
    {
      fprintf(f, "    address: %p\n", stat->reg->desc );
    }
  else
    {
      fprintf(f, "    %s (%d--%d)\n", stat->reg->desc->file_name,
	      stat->reg->desc->begin_first_line,
	      stat->reg->desc->end_first_line);
    }

  if( stat->seq )
    {
      fprintf(f, "SEQ  : ");
      mstat_dump( stat->seq, f );
    }

  if( stat->main )
    {
      fprintf(f, "MAIN : ");
      mstat_dump( stat->main, f );
    }

  if( stat->enter )
    {
      fprintf(f, "ENTER: ");
      mstat_dump( stat->enter, f );
    }

  if( stat->body )
    {
      fprintf(f, "BODY : ");
      mstat_dump( stat->body, f );
    }

   if( stat->ibarr )
     {
       fprintf(f, "IBARR: ");
       mstat_dump( stat->ibarr, f );
     }

   if( stat->exit )
     {
       fprintf(f, "EXIT : ");
       mstat_dump( stat->exit, f );
     }

   fprintf(f, "\n\n");
}


double regstat_getcputime( regstat_t *stat )
{
  double time;
  double task;
  unsigned exec;
  
  time=0.0; task=0.0;
  assert( stat->main );
  MSTAT_SUMT( stat->main, time );  
  MSTAT_SUMC( stat->main, exec );  
  if(stat->task) { 
    MSTAT_SUMT( stat->task, task );
  }
  
  //  time-=task;

  /*
  if( (stat->reg->type)==OMPP_REGION )
    time*=ompp_max_threads;
  */

  //  return time/(double)(stat->main->nexect);
  return time>0.0?time:0.0;
}

double regstat_getcputime_x( regstat_t *stat )
{
  double time=0.0;
  double incl=0.0;
  double excl=0.0;
  double task=0.0;
  double res;
  
  time = regstat_getcputime(stat);

  if( stat->main_x )
    {
      assert( stat->main );
      MSTAT_SUMT( stat->main,   incl );
      MSTAT_SUMT( stat->main_x, excl );

      /*
      incl/=(double)(stat->main->nexect);
      excl/=(double)(stat->main_x->nexect);
      */
    }

  if( stat->body_x )
    {
      assert( stat->body );
      MSTAT_SUMT( stat->body,   incl );
      MSTAT_SUMT( stat->body_x, excl );

      /*
      incl/=(double)(stat->body->nexect);
      excl/=(double)(stat->body_x->nexect);
      */
    }

  if(stat->task) { 
    MSTAT_SUMT( stat->task, task );
  }

  /*
  if( (stat->reg->type)==OMPP_REGION )
    res=time-(incl+excl)*ompp_max_threads;
  else
    res=time-incl+excl;
  */
  res=time-incl+excl/* -task*/;
  
  return res>0?res:0.0;
}




double regstat_getwtime( regstat_t *stat )
{
  double d;
  d = regstat_getcputime(stat);
  return d /(double)(stat->main->nexect);
}

double regstat_getwtime_x( regstat_t *stat )
{
  double d;
  d = regstat_getcputime_x(stat);
  return d /(double)(stat->main->nexect);
}


