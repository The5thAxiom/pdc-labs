/* 
 * ompp.c  */
/* @OMPP_HEADER@ */

#include <stdio.h>
#include <stdlib.h>    /* atexit() */
#include <string.h>    /* strcmp() */
#include <unistd.h>    /* chdir()  */
#include <assert.h>

#ifdef _MSC_VER 
#pragma warning(disable : 4996)
#include "Winsock2.h"  /* struct timeval */
#else 
#include <sys/time.h>
#endif 

#ifndef _MSC_VER
#include <libgen.h>
#endif 

#ifdef OMPP_USE_PAPI
#include <papi.h>
#include "ompp_papi.h"
#endif

#include "ompp.h"
#include "pomp_lib.h"
#include "ompp_pomp.h"
#include "ompp_report.h"
#include "ompp_properties.h"
#include "ompp_incremental.h"
#include "ompp_eval.h"
#include "ompp_rstack.h"
#include "ompp_time.h"
#include "ompp_region.h"

#include "utils/jobenv.h"

#ifdef __GNUC__
#include "ompp_gcc.h"
#endif

#ifdef OMPP_MPI
#include "ompp_mpi.h"
#endif

// 
// gives a short name for each OMPP region type as in the
// enum ompp_reg_type_t (ompp_reg_type_t starts with numeric value of 0)
// for OMPP_ATOMIC
//
const char *ompp_region_name[OMPP_LAST];
const char *ompp_region_full_name[OMPP_LAST];

ompp_status_t  ompp_status = OMPP_STATUS_NONE;
char           ompp_construct_enabled[OMPP_LAST];
unsigned       ompp_region_count[OMPP_LAST];
char          *ompp_executable_name=0;

int ompp_outformat=0;
int ompp_max_threads=0;
int ompp_concurrency=0;

char *ompp_outdir=0;

struct timeval ompp_tstart;
struct timeval ompp_tstop;
double ompp_duration;

double ompp_usertime;
double ompp_systemtime;

double ompp_tdump;
int ompp_done=0;

char ompp_hostname[80];

//ompp_region_t *ompp_current_par;

ompp_region_t **ompp_current_task;
mstat_t **ompp_task_mstat;
mstat_t **ompp_task_mstat_cg;
unsigned **ompp_task_nesting;


// a list of all regions
dllist_t ompp_region_list;

// region stack
rstack_t ompp_rstack;

int ompp_collapse_locks;
int ompp_quiet;

ompp_region_t *curr_task;

// use PID to generate unique filename instead of incrementing counters
int ompp_usepid;

int ompp_lockhash_size;

int   ompp_overwrite_reports;
char* ompp_reportfile;

char  ompp_sep=',';

unsigned ompp_version = OMPP_VERSION;


#ifdef OMPP_PROFILE_OVER_TIME
void timer_handler (int signum);
#endif

//
// --------------------------------------------------------------
// --------------------------------------------------------------
//


void POMP_Init()
{
  // XXX: concurrency issues with multiple 
  //      initialization calls...
  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }
}


void POMP_On()
{
  // XXX: concurrency issues with enabling/disabling monitoring
  //      from within one thread for all others...
  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }
  OMPP_SET_MONITOR_ON;
}


void POMP_Off()
{
  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }
  OMPP_SET_MONITOR_OFF;
}



void POMP_Begin( struct ompregdescr *r )
{
  // have to initialize even if disabled
  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }

  if( ompp_construct_enabled[OMPP_REGION] )
    ompp_pomp_region_enter( r, OMPP_REGION );
}

void POMP_End( struct ompregdescr *r )
{
  // have to initialize even if disabled
  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }

  if( ompp_construct_enabled[OMPP_REGION] )
    ompp_pomp_region_exit( r, OMPP_REGION );
}

void POMP_Atomic_enter( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_ATOMIC] )
    ompp_pomp_region_enter( r, OMPP_ATOMIC );
}

void POMP_Atomic_exit( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_ATOMIC] )
    ompp_pomp_region_exit( r, OMPP_ATOMIC );
}

void POMP_Barrier_enter( struct ompregdescr *r )
{
  ompp_pomp_region_enter( r, OMPP_BARRIER );
}

void POMP_Barrier_exit( struct ompregdescr *r )
{
  ompp_pomp_region_exit( r, OMPP_BARRIER );
}

void POMP_Critical_begin( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_CRITICAL] )
    ompp_pomp_region_enter( r, OMPP_CRITICAL_BODY );
}

void POMP_Critical_end( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_CRITICAL] )
    ompp_pomp_region_exit( r, OMPP_CRITICAL_BODY );
}

void POMP_Critical_enter( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_CRITICAL] )
    ompp_pomp_region_enter( r, OMPP_CRITICAL );
}

void POMP_Critical_exit( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_CRITICAL] )
    ompp_pomp_region_exit( r, OMPP_CRITICAL );
}

void POMP_Do_enter( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_LOOP] )
    ompp_pomp_region_enter( r, OMPP_LOOP );
}

void POMP_Do_exit( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_LOOP] )
    ompp_pomp_region_exit( r, OMPP_LOOP );
}

void POMP_Flush_enter( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_FLUSH] )
    ompp_pomp_region_enter( r, OMPP_FLUSH );
}

void POMP_Flush_exit( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_FLUSH] )
    ompp_pomp_region_exit( r, OMPP_FLUSH );
}

void POMP_For_enter( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_LOOP] )
    ompp_pomp_region_enter( r, OMPP_LOOP );
}

void POMP_For_exit( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_LOOP] )
    ompp_pomp_region_exit( r, OMPP_LOOP );
}

void POMP_Master_begin( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_MASTER] )
    ompp_pomp_region_enter( r, OMPP_MASTER );
}

void POMP_Master_end( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_MASTER] )
    ompp_pomp_region_exit( r, OMPP_MASTER );
}

void POMP_Parallel_begin( struct ompregdescr *r )
{
  if( omp_get_thread_num()==0 )
    ompp_concurrency=omp_get_num_threads();
  ompp_pomp_region_enter( r,  OMPP_PARALLEL_BODY );
}

void POMP_Parallel_end( struct ompregdescr *r )
{
  ompp_pomp_region_exit( r, OMPP_PARALLEL_BODY );
}

void POMP_Parallel_fork( struct ompregdescr *r )
{
  ompp_pomp_region_enter( r, OMPP_PARALLEL );
}

void POMP_Parallel_join( struct ompregdescr *r )
{
  ompp_pomp_region_exit( r, OMPP_PARALLEL );
  ompp_concurrency=1;
}

void POMP_Section_begin( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_SECTIONS] )
    ompp_pomp_region_enter( r, OMPP_SECTION );
}

void POMP_Section_end( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_SECTIONS] )
    ompp_pomp_region_exit( r, OMPP_SECTION );
}

void POMP_Sections_enter( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_SECTIONS] )
    ompp_pomp_region_enter( r, OMPP_SECTIONS );
}

void POMP_Sections_exit( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_SECTIONS] )
    ompp_pomp_region_exit( r, OMPP_SECTIONS );
}

void POMP_Single_begin( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_SINGLE] )
    ompp_pomp_region_enter( r, OMPP_SINGLE_BODY );
}

void POMP_Single_end( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_SINGLE] )
    ompp_pomp_region_exit( r, OMPP_SINGLE_BODY );
}

void POMP_Single_enter( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_SINGLE] )
    ompp_pomp_region_enter( r, OMPP_SINGLE );
}

void POMP_Single_exit( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_SINGLE] )
    ompp_pomp_region_exit( r, OMPP_SINGLE );
}

void POMP_Task_begin( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_TASK] )
    ompp_pomp_region_enter( r, OMPP_TASKEXEC );
}

void POMP_Task_end( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_TASK] )
    ompp_pomp_region_exit( r, OMPP_TASKEXEC );
}

void POMP_Task_enter( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_TASK] )
    ompp_pomp_region_enter( r, OMPP_TASK );
}

void POMP_Task_exit( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_TASK] )
    ompp_pomp_region_exit( r, OMPP_TASK );
}


void POMP_Utask_begin( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_UTASK] )
    ompp_pomp_region_enter( r, OMPP_UTASKEXEC );
}

void POMP_Utask_end( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_UTASK] )
    ompp_pomp_region_exit( r, OMPP_UTASKEXEC );
}

void POMP_Utask_enter( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_UTASK] )
    ompp_pomp_region_enter( r, OMPP_UTASK );
}

void POMP_Utask_exit( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_UTASK] )
    ompp_pomp_region_exit( r, OMPP_UTASK );
}


void POMP_Taskwait_enter( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_TASKWAIT] )
    ompp_pomp_region_enter( r, OMPP_TASKWAIT );
}

void POMP_Taskwait_exit( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_TASKWAIT] )
    ompp_pomp_region_exit( r, OMPP_TASKWAIT );
}


void POMP_Workshare_enter( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_WORKSHARE] )
    ompp_pomp_region_enter( r, OMPP_WORKSHARE );
}

void POMP_Workshare_exit( struct ompregdescr *r )
{
  if( ompp_construct_enabled[OMPP_WORKSHARE] )
    ompp_pomp_region_exit( r, OMPP_WORKSHARE );
}


//
// LOCKS
//

void POMP_Init_lock( omp_lock_t *lock )
{
  ompp_pomp_init_lock(lock);
}

void POMP_Destroy_lock( omp_lock_t *lock )
{
  ompp_pomp_destroy_lock(lock);
}

void POMP_Set_lock( omp_lock_t *lock )
{
  ompp_pomp_set_lock(lock);
}

void POMP_Unset_lock( omp_lock_t *lock )
{
  ompp_pomp_unset_lock(lock);
}

int POMP_Test_lock( omp_lock_t *lock )
{
  return ompp_pomp_test_lock(lock);
}

/* 
 * nestable locks 
 */

void POMP_Init_nest_lock( omp_nest_lock_t *lock )
{
  omp_init_nest_lock(lock);
}

void POMP_Destroy_nest_lock( omp_nest_lock_t *lock )
{
  omp_destroy_nest_lock(lock);
}

void POMP_Set_nest_lock( omp_nest_lock_t *lock )
{
  omp_set_nest_lock(lock);
}

void POMP_Unset_nest_lock( omp_nest_lock_t *lock )
{
  omp_unset_nest_lock(lock);
}

int POMP_Test_nest_lock( omp_nest_lock_t *lock )
{
  return omp_test_nest_lock(lock);
}



void ompp_finalize()
{
  ompp_usertime   = ompp_utime() - ompp_usertime;
  ompp_systemtime = ompp_stime() - ompp_systemtime;
  
  ompp_done = 1;

  print_report();
}

void ompp_initialize()
{
  int retval;
  int i;
  char *s;
  char buf[80];

  gettimeofday( &ompp_tstart, NULL );
  ompp_usertime   = ompp_utime();
  ompp_systemtime = ompp_stime();
  ompp_tdump = 0;
  //  ompp_current_par = 0;

  list_init( &ompp_region_list );

  //
  // short names
  //
  ompp_region_name[OMPP_ATOMIC]             = "ATOMIC"  ;
  ompp_region_name[OMPP_BARRIER]            = "BARRIER" ;
  ompp_region_name[OMPP_CRITICAL]           = "CRITICAL";
  ompp_region_name[OMPP_CRITICAL_BODY]      = "CRITBODY";
  ompp_region_name[OMPP_FLUSH]              = "FLUSH"   ;
  ompp_region_name[OMPP_LOOP]               = "LOOP"    ;
  ompp_region_name[OMPP_MASTER]             = "MASTER"  ;
  ompp_region_name[OMPP_PARALLEL]           = "PARALLEL";
  ompp_region_name[OMPP_PARALLEL_BODY]      = "PARBODY" ;
  ompp_region_name[OMPP_SECTIONS]           = "SECTIONS";
  ompp_region_name[OMPP_SECTION]            = "SECTION" ;
  ompp_region_name[OMPP_SINGLE]             = "SINGLE"  ;
  ompp_region_name[OMPP_SINGLE_BODY]        = "SINGBODY";
  ompp_region_name[OMPP_TASK]               = "TASK"    ;
  ompp_region_name[OMPP_TASKEXEC]           = "TASKEXEC";
  ompp_region_name[OMPP_UTASK]              = "UTASK"   ;
  ompp_region_name[OMPP_UTASKEXEC]          = "UTASKEXC";
  ompp_region_name[OMPP_TASKWAIT]           = "TASKWAIT";
  ompp_region_name[OMPP_WORKSHARE]          = "WRKSHARE";
  ompp_region_name[OMPP_REGION]             = "USERREG" ;
  ompp_region_name[OMPP_PARALLEL_LOOP]      = "PARLOOP" ;
  ompp_region_name[OMPP_PARALLEL_SECTIONS]  = "PARSECTS";
  ompp_region_name[OMPP_PARALLEL_WORKSHARE] = "PARWRKSH";
  ompp_region_name[OMPP_LOCK]               = "LOCK"    ;
  ompp_region_name[OMPP_LOCK_BODY]          = "LOCKBODY";
  ompp_region_name[OMPP_LOCK_FAIL]          = "LOCKFAIL";
  ompp_region_name[OMPP_UNSPECIFIED]        = "UNSPEC"  ;


  //
  // full names
  //
  ompp_region_full_name[OMPP_ATOMIC]             = "ATOMIC";
  ompp_region_full_name[OMPP_BARRIER]            = "BARRIER";
  ompp_region_full_name[OMPP_CRITICAL]           = "CRITICAL";
  ompp_region_full_name[OMPP_CRITICAL_BODY]      = "CRITICAL BODY";
  ompp_region_full_name[OMPP_FLUSH]              = "FLUSH";
  ompp_region_full_name[OMPP_LOOP]               = "LOOP";
  ompp_region_full_name[OMPP_MASTER]             = "MASTER";
  ompp_region_full_name[OMPP_PARALLEL]           = "PARALLEL";
  ompp_region_full_name[OMPP_PARALLEL_BODY]      = "PARALLEL BODY";
  ompp_region_full_name[OMPP_SECTIONS]           = "SECTIONS";
  ompp_region_full_name[OMPP_SECTION]            = "SECTION";
  ompp_region_full_name[OMPP_SINGLE]             = "SINGLE";
  ompp_region_full_name[OMPP_SINGLE_BODY]        = "SINGLE BODY";

  ompp_region_full_name[OMPP_TASK]               = "TASK";
  ompp_region_full_name[OMPP_TASKEXEC]           = "TASKEXEC";
  ompp_region_full_name[OMPP_UTASK]              = "UTASK";
  ompp_region_full_name[OMPP_UTASKEXEC]          = "UTASKEXEC";
  ompp_region_full_name[OMPP_TASKWAIT]           = "TASKWAIT";

  ompp_region_full_name[OMPP_WORKSHARE]          = "WORKSHARE";
  ompp_region_full_name[OMPP_REGION]             = "USER REGION";
  ompp_region_full_name[OMPP_PARALLEL_LOOP]      = "PARALLEL LOOP";
  ompp_region_full_name[OMPP_PARALLEL_SECTIONS]  = "PARALLEL SECTIONS";
  ompp_region_full_name[OMPP_PARALLEL_WORKSHARE] = "PARALLEL WORKSHARE";
  ompp_region_full_name[OMPP_LOCK]               = "LOCK";
  ompp_region_full_name[OMPP_LOCK_BODY]          = "LOCK BODY";
  ompp_region_full_name[OMPP_LOCK_FAIL]          = "LOCK FAIL";
  ompp_region_full_name[OMPP_UNSPECIFIED]        = "UNSPECIFIED";


  gethostname( ompp_hostname, 80 );

  //
  // initialize the "enabled" list and the region count list
  //
  for( i=0; i<OMPP_LAST; i++ )
    {
      ompp_construct_enabled[i]=1;
      ompp_region_count[i]=0;
    }

  //
  // disable constructs based on environment settings
  //
  if( (s=getenv("OMPP_DISABLE_ATOMIC"))!=0 && strcmp(s, "0") )
    ompp_construct_enabled[OMPP_ATOMIC]=0;

  if( (s=getenv("OMPP_DISABLE_BARRIER"))!=0 && strcmp(s, "0") )
    ompp_construct_enabled[OMPP_BARRIER]=0;

  if( (s=getenv("OMPP_DISABLE_CRITICAL"))!=0 && strcmp(s, "0") )
    ompp_construct_enabled[OMPP_CRITICAL]=0;

  if( (s=getenv("OMPP_DISABLE_FLUSH"))!=0 && strcmp(s, "0") )
    ompp_construct_enabled[OMPP_FLUSH]=0;
  
  if( (s=getenv("OMPP_DISABLE_LOOP"))!=0 && strcmp(s, "0") )
    ompp_construct_enabled[OMPP_LOOP]=0;

  if( (s=getenv("OMPP_DISABLE_MASTER"))!=0 && strcmp(s, "0") )
    ompp_construct_enabled[OMPP_MASTER]=0;

  //if( (s=getenv("OMPP_DISABLE_PARALLEL"))!=0 && strcmp(s, "0") )
  //  ompp_construct_enabled[OMPP_PARALLEL]=0;

  if( (s=getenv("OMPP_DISABLE_SECTIONS"))!=0 && strcmp(s, "0") )
    ompp_construct_enabled[OMPP_SECTIONS]=0;

  if( (s=getenv("OMPP_DISABLE_SINGLE"))!=0 && strcmp(s, "0") )
    ompp_construct_enabled[OMPP_SINGLE]=0;

  if( (s=getenv("OMPP_DISABLE_WORKSHARE"))!=0 && strcmp(s, "0") )
    ompp_construct_enabled[OMPP_WORKSHARE]=0;

  if( (s=getenv("OMPP_DISABLE_USER_REGION"))!=0 && strcmp(s, "0") )
    ompp_construct_enabled[OMPP_REGION]=0;

  //if( (s=getenv("OMPP_DISABLE_PARALLEL_SECTIONS"))!=0 && strcmp(s, "0") )
  //  ompp_construct_enabled[OMPP_PARALLEL_SECTIONS]=0;

  //if( (s=getenv("OMPP_DISABLE_PARALLEL_LOOP"))!=0 && strcmp(s, "0") )
  //  ompp_construct_enabled[OMPP_PARALLEL_LOOP]=0;

  //if( (s=getenv("OMPP_DISABLE_PARALLEL_WORKSHARE"))!=0 && strcmp(s, "0") )
  //  ompp_construct_enabled[OMPP_PARALLEL_WORKSHARE]=0;

  if( (s=getenv("OMPP_DISABLE_LOCK"))!=0 && strcmp(s, "0") )
    ompp_construct_enabled[OMPP_LOCK]=0;

  if( (s=getenv("OMPP_DISABLE_LOCKS"))!=0 && strcmp(s, "0") )
    ompp_construct_enabled[OMPP_LOCK]=0;

  if( (s=getenv("OMPP_DISABLE_UNTIED"))!=0 && strcmp(s, "0") )
    {
      ompp_construct_enabled[OMPP_UTASK]=0;
    }

  //
  // set the name of the target application
  //
  if( (s=getenv("OMPP_APPNAME"))!=0 )
    {
      ompp_executable_name = basename(s);
    }
  else
    {
      /* XXX this code is broken, needs to be fixed 

      char buf1[MAXSIZE_CMDLINE];
      char buf2[MAXSIZE_CMDLINE];
      int sz;

      get_cmdline(buf1,buf2);
      sz=strlen(buf1);
      while(sz>0 && buf1[sz]==' ') {
	buf1[sz]=0; sz--;
      } 

      ompp_executable_name = strdup(basename(buf1));
      
      if( strlen(ompp_executable_name)==0 ) {
      ompp_executable_name = strdup("unknown");
      }
      */

      ompp_executable_name = strdup("unknown");
      
    }

  //
  // determine desired output format
  //
  if( (s=getenv("OMPP_OUTFORMAT"))!=0 && 
      (!strncmp(s, "CSV", 3) || !strncmp(s, "csv", 3)) )
    {
      ompp_outformat=OMPP_OUTFORMAT_CSV;
    }
  else if( (s=getenv("OMPP_OUTFORMAT"))!=0 && 
      (!strncmp(s, "CUBE", 4) || !strncmp(s, "cube", 4)) )
    {
      ompp_outformat=OMPP_OUTFORMAT_CUBE;
    }
  else if( (s=getenv("OMPP_OUTFORMAT"))!=0 && 
      (!strncmp(s, "XML", 3) || !strncmp(s, "xml", 3)) )
    {
      ompp_outformat=OMPP_OUTFORMAT_XML;
    }

  else
    {
      ompp_outformat=OMPP_OUTFORMAT_PLAIN;
    }
  
  //
  // output directory
  //
  if( (s=getenv("OMPP_OUTDIR")) )
    {
      ompp_outdir=strdup(s);
    }


  //
  // set the number of threads
  //
  ompp_max_threads = omp_get_max_threads();
  
  ompp_concurrency = 1;

  //
  // check the collapse locks flag
  //
  ompp_collapse_locks = 0;
  if( (s=getenv("OMPP_COLLAPSE_LOCKS"))!=0 && strcmp(s, "0") )
    ompp_collapse_locks = 1;
  
  ompp_quiet=0;
  if( (s=getenv("OMPP_QUIET"))!=0 && strcmp(s, "0") )
    ompp_quiet=1;

  ompp_usepid=0;
  if( (s=getenv("OMPP_USEPID"))!=0 && strcmp(s, "0") )
    ompp_usepid=1;

  ompp_lockhash_size=1000;
  if( (s=getenv("OMPP_LOCKHASH"))!=0 && strcmp(s, "0") )
    ompp_lockhash_size=atoi(s);

  ompp_overwrite_reports=0;
  if( (s=getenv("OMPP_OVERWRITE"))!=0 && strcmp(s, "0") )
    {
      ompp_overwrite_reports=1;
    }

  //
  // check if user wants a sepecific report file
  //
  ompp_reportfile = 0;
  if( (s=getenv("OMPP_REPORTFILE"))!=0 )
    {
      ompp_reportfile=strdup(s);
      ompp_overwrite_reports=1;
    }
  

  if( ompp_outdir )
    {
      if( chdir(ompp_outdir) && !ompp_quiet )
	{
	  fprintf(stderr, "ompP: error changing directory to '%s'\n", 
		  ompp_outdir);
	}
    }

  // initialize PAPI
#ifdef OMPP_USE_PAPI
  ompp_papi_initialize();
#endif

  // initizalize the POMP part
  ompp_pomp_initialize();

  // initialize properties part
  ompp_perfprop_initialize();

  // initialize PoT part
  ompp_profile_over_time_initialize();

#ifdef __GNUC__
  ompp_gcc_initialize();
#endif

#ifdef OMPP_MPI
  ompp_mpi_initialize();
#endif 

  rstack_init( &ompp_rstack, ompp_max_threads ); 

  ompp_current_task = (ompp_region_t**) 
    malloc(sizeof(ompp_region_t*)*ompp_max_threads);
  ompp_task_mstat = (mstat_t**) 
    malloc(sizeof(mstat_t*)*ompp_max_threads);
  ompp_task_mstat_cg = (mstat_t**) 
    malloc(sizeof(mstat_t*)*ompp_max_threads);
  ompp_task_nesting = (unsigned**) 
    malloc(sizeof(unsigned*)*ompp_max_threads);

  for( i=0; i<ompp_max_threads; i++ )
    {
      ompp_current_task[i] = 0;
      ompp_task_mstat[i] = 0;
      ompp_task_nesting[i] = 0;
    }

  // register handler for program exit
  atexit(ompp_finalize);

  ompp_status |= OMPP_STATUS_INITIALIZED;
  ompp_status |= OMPP_STATUS_MONITOR_ON;
}


// XXX: move to util.c
void lldtostr(char *buf, long long n)
{
  int i, j, len;
  char temp[40];

  sprintf(temp, "%lld", n);
  len = strlen(temp);

  i=0; j=0;

  // copy initial minus sign
  if( temp[i]=='-' )
    buf[i++]=temp[j++];

  // avoid initial dot
  if( ((len-i)%3)==0 )
    buf[i++]=temp[j++];

  for( ; i<len; i++ )
    {
      if( ((len-i)%3)==0 )
        {
          buf[j++]='.';
        }
      buf[j++]=temp[i];

    }
  buf[j++]=0;
}



