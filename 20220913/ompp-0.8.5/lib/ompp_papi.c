/*
 * ompp_papi.c  */
/* @OMPP_HEADER@ */

#ifdef OMPP_USE_PAPI

#include <stdio.h>
#include <assert.h>
#include <papi.h>
#include <string.h>
#include "ompp.h"
#include "ompp_papi.h"
#include "ompp_eval.h"

//XXX
#include <pthread.h>

omp_lock_t papi_lock;


/* user-specified names of hw counters */
char  *ompp_papi_ctr_name[OMPP_PAPI_MAX_CTRS];

/* user-specified names of evaluators and corresponding
   evaluator objects */
char  *ompp_eval_string[OMPP_MAX_EVALUATORS];
void  *ompp_eval_evaluator[OMPP_MAX_EVALUATORS];

/* init flag per thread */
int   *ompp_papi_initialized;


/* tracks the names of counters to be used in evaluators */
char  *ompp_papi_ctr_eval_name[OMPP_PAPI_MAX_CTRS+2];

int    ompp_papi_ctr_code[OMPP_PAPI_MAX_CTRS];
/* one evtset per thread */
int   *ompp_papi_ctr_evtset;

/* DS-oriented monitoring */
#ifdef OMPP_DATARANGE

PAPI_option_t ompp_papi_opt;
void     *ompp_papi_daddr;
size_t    ompp_papi_dsize;
char     *ompp_papi_dname;
size_t    ompp_papi_doffs; /* start offset */
size_t    ompp_papi_doffe; /* end offset */

#endif /* OMPP_DATARANGE */


void ompp_papi_initialize()
{
  int i;
  char *s;
  int retval;
  char buf[120];
  int ncomp;

  omp_init_lock( &papi_lock );

#ifdef OMPP_DATARANGE

  ompp_papi_daddr = 0;
  ompp_papi_dsize = 0;
  ompp_papi_dname = 0;

#endif /* OMPP_DATARANGE */
  
  
  // find out if user wants to use counters at all...
  // - first check if user specified counters him/her self
  for( i=0; i<OMPP_PAPI_MAX_CTRS; i++ )
    {
      ompp_papi_ctr_name[i]=0;
      
      sprintf( buf, "OMPP_CTR%d", i+1);

      if( (s=getenv(buf))!=0 && (*s)!=0 )
	{
	  // yes, we're using PAPI!
	  ompp_status |= OMPP_STATUS_PAPI_ON;
	  ompp_papi_ctr_name[i] = strdup(s);
	}
    }


  // - then check if user specified evaluators
  for( i=0; i<OMPP_MAX_EVALUATORS; i++ )
    {
      ompp_eval_string[i]=0;
      ompp_eval_evaluator[i]=0;
      
      sprintf( buf, "OMPP_EVAL%d", i+1 );

      if( (s=getenv(buf))!=0 && (*s)!=0 )
	{
	  ompp_eval_evaluator[i] = evaluator_create(s);
	  if( !ompp_eval_evaluator[i] )
	    {
	      fprintf(stderr, "ompP: Error in evaluator '%s'\n", s );
	      continue;
	    }

	  ompp_eval_string[i]=strdup(s);

	  // now analyze evaluator string and add counters
	  ompp_eval_find_ctrs( s, ompp_papi_ctr_name );
	}
    }
  
  // - check again if counters are on and make a copy 
  //   of the counter names in ompp_papi_ctr_eval_name for use with
  //   the evaluators 
  for( i=0; i<OMPP_PAPI_MAX_CTRS; i++ )
    {
      if( ompp_papi_ctr_name[i] ) 
	{
	  ompp_status |= OMPP_STATUS_PAPI_ON;
	  ompp_papi_ctr_eval_name[i] = ompp_papi_ctr_name[i]; 
	}
      else
	{
	  ompp_papi_ctr_eval_name[i] = strdup("");
	}
    }
  
  // - add the two special purpose constants to be used in counters
  ompp_papi_ctr_eval_name[OMPP_PAPI_MAX_CTRS]   = "EXECT";
  ompp_papi_ctr_eval_name[OMPP_PAPI_MAX_CTRS+1] = "EXECC";

  
  // - do the actual PAPI initialization if counters selected
  if( OMPP_IS_PAPI_ON )
    {
      retval=PAPI_library_init( PAPI_VER_CURRENT );
      
      if( retval!=PAPI_VER_CURRENT && retval>0 )
	{
	  if( !ompp_quiet )
	    fprintf(stderr, "ompP: PAPI library version mismatch\n");
	  exit(1);
	}
      if( retval<0 )
	{
	  if( !ompp_quiet )
	    fprintf(stderr, "ompP: PAPI initialization error\n");
	  exit(1);
	}      

      /* XXX    if( PAPI_thread_init( (unsigned long(*)(void))omp_get_thread_num )!=PAPI_OK ) */
      if( PAPI_thread_init( pthread_self )!=PAPI_OK )
	{
	  if( !ompp_quiet )
	    fprintf(stderr, "ompP: PAPI thread initialization error\n");
	  exit(1);
	}
      
#ifdef OMPP_USE_CPAPI
      ncomp = PAPI_num_components();
#else

      ompp_papi_ctr_evtset  = (int*) malloc( sizeof(int)*ompp_max_threads ); 
      ompp_papi_initialized = (int*) malloc( sizeof(int)*ompp_max_threads );
      
      assert(ompp_papi_ctr_evtset);
      assert(ompp_papi_initialized);
      
      for( i=0; i<ompp_max_threads; i++ )
	{
	  ompp_papi_ctr_evtset[i]  = PAPI_NULL;    
	  ompp_papi_initialized[i] = 0;
	}
      
      for( i=0; i<OMPP_PAPI_MAX_CTRS; i++ )
	{
	  if( ompp_papi_ctr_name[i] ) 
	    {
	      PAPI_event_name_to_code( ompp_papi_ctr_name[i], 
				       &ompp_papi_ctr_code[i] );
	      
	      if( PAPI_query_event( ompp_papi_ctr_code[i] )!=PAPI_OK )
		{
		  if( !ompp_quiet )
		    fprintf(stderr, "ompP: PAPI name-to-code error: %s\n",
			    ompp_papi_ctr_name[i]);
		  exit(1);
		}
	      else
		{
		  if( !ompp_quiet )
		    fprintf(stderr, "ompP: successfully registered counter %s\n", 
			    ompp_papi_ctr_name[i]);
		}
	    }
	}
#endif
      
    }
}


void ompp_papi_thread_init(int tid)
{
  int i;
  long long ctr[OMPP_PAPI_MAX_CTRS];
  char *s1, *s2, *s3;

  omp_set_lock( &papi_lock );
  
  if( !(OMPP_IS_PAPI_ON) )
    return;

  if( PAPI_create_eventset(&(ompp_papi_ctr_evtset[tid])) != PAPI_OK )
    {
      if ( !ompp_quiet )
	fprintf(stderr, "ompP: Error creating eventset (tid=%d)\n", tid);
    }


#ifdef OMPP_DATARANGE

  if( (s1=getenv("OMPP_DATA_ADDR")) &&
      (s2=getenv("OMPP_DATA_SIZE")) )
    {
      ompp_papi_daddr = (char*) strtol(s1, 0, 0);
      ompp_papi_dsize = atoi(s2);
      if( s3=getenv("OMPP_DATA_NAME") ) 
	{
	  ompp_papi_dname = strdup(s3);
	}
    }


  if( (ompp_papi_daddr>0) && (ompp_papi_dsize>0) )
    {
      ompp_papi_opt.addr.eventset = ompp_papi_ctr_evtset[tid];
      
      ompp_papi_opt.addr.start = ompp_papi_daddr;
      ompp_papi_opt.addr.end   = ompp_papi_opt.addr.start+ompp_papi_dsize;
      
      /*
      fprintf(stderr, "addresses = %p -- %p \n", ompp_papi_opt.addr.start,
              ompp_papi_opt.addr.end);
      */

      if( PAPI_set_opt(PAPI_DATA_ADDRESS, &ompp_papi_opt) != PAPI_OK )
	{
          if ( !ompp_quiet )
	    fprintf(stderr, "ompP: Error adding data addresse rang option (tid=%d)\n", tid);
	}

      ompp_papi_doffs = ompp_papi_opt.addr.start_off;
      ompp_papi_doffe = ompp_papi_opt.addr.end_off;
    }

#endif /* OMPP_DATARANGE */



  for( i=0; i<OMPP_PAPI_MAX_CTRS; i++ )
    {
      if( !ompp_papi_ctr_name[i] )
	continue;
      
      if( PAPI_add_event(ompp_papi_ctr_evtset[tid], 
			 ompp_papi_ctr_code[i]) != PAPI_OK )
	{
	  if ( !ompp_quiet )
	    {
	      fprintf(stderr, "ompP: Error adding event to eventset (tid=%d): %s\n", 
		
		      tid, ompp_papi_ctr_name[i] );
	      exit(1);
	    }
	}      
      
    }
  
  if( PAPI_start(ompp_papi_ctr_evtset[tid]) != PAPI_OK )
    {
      if ( !ompp_quiet )
	fprintf(stderr, "ompP: Error starting eventset (tid=%d)\n", tid);
    }
  
  ompp_papi_initialized[tid]=1;
  
  omp_unset_lock( &papi_lock );
}


void ompp_papi_thread_stop(int tid)
{
    int ret;
    long long ctr[OMPP_PAPI_MAX_CTRS];

    omp_set_lock( &papi_lock );

    if( (ret=PAPI_stop(ompp_papi_ctr_evtset[tid], ctr)) != PAPI_OK )
    {
	if ( !ompp_quiet )
	    fprintf(stderr, "ompP: Error stopping eventset (tid=%d)\n", tid);
    }
    
    if( (ret=PAPI_cleanup_eventset( ompp_papi_ctr_evtset[tid])) != PAPI_OK )
    {
	if ( !ompp_quiet )
	    fprintf(stderr, "ompP: Error cleaning-up eventset (tid=%d)\n", tid);
    }

    if( (ret=PAPI_destroy_eventset(&(ompp_papi_ctr_evtset[tid]))) != PAPI_OK )
    {
	if ( !ompp_quiet )
	    fprintf(stderr, "ompP: Error destroying eventset (tid=%d)\n", tid);
    }

    if( (ret=PAPI_unregister_thread()) != PAPI_OK )
    {
	if ( !ompp_quiet )
	    fprintf(stderr, "ompP: Error in unregister thread (tid=%d, ret=%d)\n", tid, ret);
    }

    ompp_papi_initialized[tid]=0;

    omp_unset_lock( &papi_lock );
}


void OMPP_papi_set_drange( void* addr, size_t size, char* name )
{

#ifdef OMPP_DATARANGE

  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }

  
  if( ompp_papi_daddr && !ompp_quiet )
    {
      fprintf(stderr, "ompP: Data address range set already, ignoring.\n");
      return;
    }
  
  ompp_papi_daddr = addr;
  ompp_papi_dsize = size;
  ompp_papi_dname = strdup(name);

#endif

}

#endif
