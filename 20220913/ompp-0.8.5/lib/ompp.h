/* 
 * ompp.h */
/* @OMPP_HEADER@ */

#ifndef OMPP_H_INCLUDED
#define OMPP_H_INCLUDED

#include "utils/list.h"
#include "ompp_rstack.h"
#include "pomp_lib.h"


#ifdef __cplusplus
extern "C" {
#endif
  
  //
  // OMPP region types
  //
  typedef enum
    {
      OMPP_PARALLEL=0,           // 0
      OMPP_PARALLEL_LOOP,
      OMPP_PARALLEL_SECTIONS,
      OMPP_PARALLEL_WORKSHARE,
      OMPP_LOOP,
      OMPP_SECTIONS,             // 5
      OMPP_SINGLE,
      OMPP_WORKSHARE,
      OMPP_REGION,
      OMPP_ATOMIC,
      OMPP_BARRIER,              // 10              
      OMPP_CRITICAL,
      OMPP_LOCK,
      OMPP_MASTER,
      OMPP_FLUSH,
      OMPP_SECTION,              // 15
      OMPP_TASK,
      OMPP_TASKEXEC,
      OMPP_UTASK,
      OMPP_UTASKEXEC,
      OMPP_TASKWAIT,
      OMPP_CRITICAL_BODY,
      OMPP_PARALLEL_BODY,
      OMPP_SINGLE_BODY,
      OMPP_LOCK_BODY,            
      OMPP_LOCK_FAIL,
      OMPP_UNSPECIFIED,
      OMPP_FINALIZE,
      OMPP_LAST
    }
  ompp_reg_type_t;

  
  //
  // short name and full name 
  // of ompp region types
  //
  extern const char* ompp_region_name[];
  extern const char* ompp_region_full_name[];

  //
  // controls activated / deactivated monitoring for 
  // individual construct types (e.g., monitoring can be 
  // disabled for all locks, all critical sections, etc.)
  //
  extern char ompp_construct_enabled[OMPP_LAST];


  //
  // the name of the application for which 
  // performance data is collected
  //
  extern char *ompp_executable_name;
  

  //
  // ompP output format currently PLAIN text and 
  // comma-separated value format (CSV) are supported
  //
  extern int ompp_outformat;

  extern char ompp_sep;

  extern char ompp_hostname[];
  
  extern unsigned ompp_version;
  
#define OMPP_VERSION_NUMBER(maj_,min_,rev_) \
      (((maj_<<24) | ((min_<<16) | (rev_))))

  /* -- */


#ifndef OMPP_RELEASE
#define OMPP_VERSION  OMPP_VERSION_NUMBER(0, 8, 4)
#else
#define OMPP_VERSION OMPP_VERSION_NUMBER(0, 8, 99)
#endif

  
#define OMPP_VERSION_MAJOR     ((ompp_version>>24) &   0xFF)
#define OMPP_VERSION_MINOR     ((ompp_version>>16) &   0xFF)
#define OMPP_VERSION_REVISION  ((ompp_version)     & 0xFFFF)

#define OMPP_BUILD    (__DATE__ " " __TIME__)


#define OMPP_OUTFORMAT_PLAIN   (0)
#define OMPP_OUTFORMAT_CSV     (1)
#define OMPP_OUTFORMAT_CUBE    (2)
#define OMPP_OUTFORMAT_XML     (3)



#define OMPP_GET_NUM_THREADS(n__) \
{                                 \
  n__ = omp_get_num_threads();    \
}


#ifdef OMPP_USE_GETTID

#define OMPP_GET_THREAD_NUM(n__) \
{                                \
  n__ = syscall(224)-getpid();   \
}

#else

#define OMPP_GET_THREAD_NUM(n__) \
{                                \
  n__ = omp_get_thread_num();        \
}

#endif

/*   if( n__>0 ) (n__)--;           */

  //
  // ompP output directory
  //
  extern char* ompp_outdir;

  //
  // the number of threads used by the application
  // 
  extern int ompp_max_threads;

  //
  // current number of active threads
  //
  extern int ompp_concurrency;


  //
  // time-stamps for start and stop and 
  // the duration of the program run (in seconds)
  //
  extern struct timeval ompp_tstart;
  extern struct timeval ompp_tstop;
  extern double ompp_tdump;
  extern double ompp_duration;
  extern double ompp_usertime;
  extern double ompp_systemtime;
  extern int    ompp_done;


  // 
  //  the status of the monitoring library
  //
  typedef unsigned ompp_status_t;
  extern ompp_status_t ompp_status;

#define OMPP_STATUS_NONE                 (0)
#define OMPP_STATUS_INITIALIZED          (1)
#define OMPP_STATUS_MONITOR_ON        (1<<1)
#define OMPP_STATUS_PAPI_ON           (1<<2)
#define OMPP_STATUS_WITH_POT          (1<<3)  /* profile over time */



#define OMPP_SET_MONITOR_ON   ((ompp_status |= OMPP_STATUS_MONITOR_ON))
#define OMPP_SET_MONITOR_OFF  ((ompp_status &= ~(OMPP_STATUS_MONITOR_ON)))

#define OMPP_SET_WITH_POT_ON  ((ompp_status |= OMPP_STATUS_WITH_POT))
#define OMPP_SET_WITH_POT_OFF ((ompp_status &= ~(OMPP_STATUS_WITH_POT)))



  //
  // macros that check the status
  //
#define OMPP_IS_INITIALIZED  ((ompp_status & OMPP_STATUS_INITIALIZED))
#define OMPP_IS_MONITOR_ON   ((ompp_status & OMPP_STATUS_MONITOR_ON))
#define OMPP_IS_PAPI_ON      ((ompp_status & OMPP_STATUS_PAPI_ON))
#define OMPP_IS_WITH_POT     ((ompp_status & OMPP_STATUS_WITH_POT))

#define OMPP_MAX_RECURSION 20

  
  //
  // list of all encountered regions
  //
  extern dllist_t ompp_region_list;

  //
  // region stack of all entered regions
  //
  extern rstack_t ompp_rstack;


  //
  // counts the number of regions per region type
  //
  extern unsigned ompp_region_count[OMPP_LAST];


  //
  // points to the currently active parallel region 
  // XXX: remove once we have proper region stacks
  //
  //extern struct ompp_region *ompp_current_par;

  //
  // whether to collapse statistics for LOCKS (default: yes)
  //
  extern int ompp_collapse_locks;

  //
  // wheter to overwrite existing openmp report files
  // sequence numbers are appended otherwise
  //
  extern int ompp_overwrite_reports;

  //
  // the name of the ompp report file
  //
  extern char* ompp_reportfile;

  //
  // setting this to 1 will cause ompp to give absolutely
  // no output on stderr
  //
  extern int ompp_quiet;

  //
  // use the applications PID to generate unique 
  // report filename (1=yes, 0=no -- incrementing counter
  // is used instead)
  //
  extern int ompp_usepid;

  //
  // size of the hash table used to hash OpenMP locks
  //
  extern int ompp_lockhash_size;

  
  //
  // current_task executed by a thread
  //
  extern struct ompp_region **ompp_current_task;

  //
  // worksharing mstat a task is executed in
  //
  extern struct mstat **ompp_task_mstat;
  extern struct mstat **ompp_task_mstat_cg;

  extern unsigned **ompp_task_nesting; 


/*
 * *********************************
 * data structures required for PAPI
 * *********************************
 */
#ifdef OMPP_USE_PAPI

/*
 * max number of counters supported
 */
#define OMPP_PAPI_MAX_CTRS     (4)


/*
 * max number of arithmetic expression evaluators
 */
#define OMPP_MAX_EVALUATORS    (4)

/*
 * a separate event-set is used for each thread
 */
extern int *ompp_papi_ctr_evtset;

/*
 * PAPI name for the counters 
 */
extern char* ompp_papi_ctr_name[OMPP_PAPI_MAX_CTRS];
extern char* ompp_papi_ctr_eval_name[OMPP_PAPI_MAX_CTRS+2];

/*
 * PAPI code for the counters
 */
extern int   ompp_papi_ctr_code[OMPP_PAPI_MAX_CTRS];


/*
 * strings given for each evaluator
 */
extern char *ompp_eval_string[OMPP_MAX_EVALUATORS];


/*
 * evaluator 'object' 
 */
extern void *ompp_eval_evaluator[OMPP_MAX_EVALUATORS];


/*
 * per-thread initialization flag
 */
extern int *ompp_papi_initialized;


/*
 * data structure oriented profiling stuff
 * currently works on Itanium only 
 */

#ifdef OMPP_DATARANGE
  extern void    *ompp_papi_daddr;
  extern size_t   ompp_papi_dsize;
  extern char    *ompp_papi_dname;
  extern size_t   ompp_papi_doffs;
  extern size_t   ompp_papi_doffe;
#endif /* OMPP_DATARANGE */

#endif /* OMPP_USE_PAPI */


#ifdef OMPP_MPI
extern int ompp_mpi_myrank;
extern int ompp_mpi_nprocs;

extern int *ompp_mpi_nsend;
extern int *ompp_mpi_nrecv;
extern int *ompp_mpi_ncoll;
extern int *ompp_mpi_bout;
extern int *ompp_mpi_bin;
extern double *ompp_mpi_time;
#endif /* OMPP_MPI */


  void ompp_finalize(void);
  void ompp_initialize(void);
  

  void lldtostr(char *buf, long long n);


#ifdef __cplusplus
}
#endif


#endif /* OMPP_H_INCLUDED */


