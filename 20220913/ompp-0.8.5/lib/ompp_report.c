/*
 * ompp_report.c  */ 
/* @OMPP_HEADER@ */


#include <stdio.h>
#include <assert.h>

#ifndef _MSC_VER
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#else
#include <winsock2.h>
#pragma warning(disable : 4996)
#define F_OK 0    /* XXX */ 
#endif

#include "ompp_report.h"
#include "ompp_report_callgraph.h"
#include "utils/list.h"
#include "ompp_region.h"
#include "ompp_overheads.h"
#include "ompp_properties.h"
#include "ompp_rstack.h"
#include "ompp_report_cube.h"
#include "ompp_report_xml.h"

#ifdef OMPP_USE_PAPI
#include "matheval/matheval.h"
#endif

#ifdef OMPP_CONTROLFLOW
#include "ompp_controlflow.h"
#endif

void print_report()
{
  char fname[300];
  FILE *f;
  int i;
  int pid;
  double tbegin, tend;  
  unsigned npara, nreg, nprop;
  overheads_t *ovhds;
  ompp_perfprop_t *props;
  
  
  if( ompp_outformat==OMPP_OUTFORMAT_XML )
    {	 
      i=0;
      do
	{
#ifdef OMPP_MPI
	  sprintf(fname, "%s:%d.%d-%d.ompp.xml", ompp_executable_name, 
		  ompp_mpi_myrank, ompp_max_threads, i);
#else
	  sprintf(fname, "%s.%d-%d.ompp.xml", ompp_executable_name, 
		  ompp_max_threads, i);
#endif
	  i++;
	}
      while( !access(fname, F_OK) );
      
      f = fopen( fname, "w" );
      
      ompp_report_xml( f, 
		       &ompp_region_list,
		       &ompp_rstack );
      fclose(f);
      return;
    }
  
  if( ompp_outformat==OMPP_OUTFORMAT_CUBE )
    {	 
      i=0;
      do
	{
#ifdef OMPP_MPI
	  sprintf(fname, "%s:%d.%d-%d.ompp.cube", ompp_executable_name, 
		  ompp_mpi_myrank, ompp_max_threads, i);
#else
	  sprintf(fname, "%s.%d-%d.ompp.cube", ompp_executable_name, 
		  ompp_max_threads, i);
#endif
	  i++;
	}
      while( !access(fname, F_OK) );

      f = fopen( fname, "w" );

      ompp_report_cube3( f, 
			 &ompp_region_list,
			 &ompp_rstack );

      fclose(f);

      return;
    }


  gettimeofday( &ompp_tstop, NULL );

  pid = getpid();

  tbegin = ompp_tstart.tv_sec + 1.0e-6 * ompp_tstart.tv_usec;
  tend = ompp_tstop.tv_sec + 1.0e-6 * ompp_tstop.tv_usec;

  ompp_duration = tend - tbegin;

  // user has requested specific report file name
  if( ompp_reportfile )
    {
#ifdef OMPP_MPI
      sprintf(fname, "%s:%d", ompp_reportfile, ompp_mpi_myrank);
#else
      sprintf(fname, "%s", ompp_reportfile);
#endif
      
      if( !access(fname, F_OK) &&
	  !ompp_overwrite_reports )
	{
	  i=0;
	  do
	    {

#ifdef OMPP_MPI
	      sprintf(fname, "%s:%d-%d", ompp_reportfile, 
		      ompp_mpi_myrank, i++);
#else
	      sprintf(fname, "%s-%d", ompp_reportfile, i++);
#endif
	    }
	  while( !access(fname, F_OK) );
	}
    }
  else
    {
      if( ompp_overwrite_reports )
	{
	  switch( ompp_outformat )
	    {
	    case OMPP_OUTFORMAT_CSV:
#ifdef OMPP_MPI
	      sprintf(fname, "%s:%d.%d.ompp.csv", ompp_executable_name, 
		      ompp_mpi_myrank, ompp_max_threads);
#else
	      sprintf(fname, "%s.%d.ompp.csv", ompp_executable_name, 
		      ompp_max_threads);
#endif

	      break;
	      
	    case OMPP_OUTFORMAT_PLAIN:
#ifdef OMPP_MPI
	      sprintf(fname, "%s:%d.%d.ompp.txt", ompp_executable_name, 
		      ompp_mpi_myrank, ompp_max_threads);
#else
	      sprintf(fname, "%s.%d.ompp.txt", ompp_executable_name, 
		      ompp_max_threads);
#endif
	      break;
	    }
	}
      else
	{
	  i=0;
	  do
	    {
	      const char *fext;

	      switch( ompp_outformat )
		{
		case OMPP_OUTFORMAT_CSV:
		  fext="csv";
		  break;
		case OMPP_OUTFORMAT_PLAIN:
		  fext="txt";
		  break;
		}
	      
	      if( ompp_usepid )
		{
		  sprintf(fname, "%s.%d.%d-%d.ompp.%s", ompp_executable_name, 
			  ompp_max_threads, pid, i++, fext);
		}
	      else
		{
#ifdef OMPP_MPI
		  sprintf(fname, "%s:%d.%d-%d.ompp.%s", ompp_executable_name,
			  ompp_mpi_myrank, ompp_max_threads, i++, fext);
#else
		  sprintf(fname, "%s.%d-%d.ompp.%s", ompp_executable_name, 
			  ompp_max_threads, i++, fext);
#endif
		}
	    }
	  while( !access(fname, F_OK) );
	}
    }

  
  f = fopen( fname, "w" );
  if( !f )
    {
      if( !ompp_quiet )
	fprintf( stderr, "ompP: cannot open report file '%s' for writing\n",
		 fname );
      
      return;      
    }
  



  print_header(f);
  
  print_region_list(f, &ompp_region_list);

  print_callgraph( f, &ompp_rstack );

  print_region_stats(f, &ompp_region_list );

  print_callgraph_profiles( f, &ompp_rstack );

  // total number of parallel region
  npara = ompp_region_count[OMPP_PARALLEL]+
    ompp_region_count[OMPP_PARALLEL_LOOP]+
    ompp_region_count[OMPP_PARALLEL_SECTIONS]+
    ompp_region_count[OMPP_PARALLEL_WORKSHARE];

  // total number of regions
  nreg=0;
  for( i=0; i<OMPP_LAST; i++ )
    {
      nreg+=ompp_region_count[i];
    }

  ovhds = (overheads_t*) malloc(sizeof(overheads_t)*npara);
  assert(ovhds);
  for( i=0; i<npara; i++ )
    {
      overheads_init( &(ovhds[i]) );
    }

  props = (ompp_perfprop_t*) malloc( sizeof(ompp_perfprop_t)*nreg );
  assert(props);

  analyze_overheads( &ompp_region_list, &ompp_rstack, ovhds, npara );
  print_overhead_report(f, ovhds, npara );

  //nprop = ompp_perfprop_check( &ompp_region_list, props, nreg );
  //  print_properties_report( f, props, nprop );

#ifdef OMPP_CONTROLFLOW
  flowinfo_print_levels(ompp_executable_name,
			&ompp_rstack);
  
  flowinfo_print_full(ompp_executable_name,
		      &ompp_rstack);

  flowinfo_print_scale(ompp_executable_name, 10);
		       
#endif  /* OMPP_CONTROLFLOW */

  fclose(f);
}


void print_header( FILE *f )
{
  int nctrs=0;
  int nevals=0;
  int i;

#ifdef OMPP_USE_PAPI
  // calculate number of used counters and evaluators
  
  for( i=0; i<OMPP_PAPI_MAX_CTRS; i++ )
    {
      if( ompp_papi_ctr_name[i]!=0 )
	nctrs++;
    }
  
  for( i=0; i<OMPP_MAX_EVALUATORS; i++ )
    {
      if( ompp_eval_string[i]!=0 )
	nevals++;
    }
#endif
  
  switch( ompp_outformat )
    {
      const struct tm *nowstruct;
      char nowstr[128];
      
    case OMPP_OUTFORMAT_PLAIN:
      
      fprintf(f, "----------------------------------------------------------------------\n");
      fprintf(f, "----     ompP General Information     --------------------------------\n");
      fprintf(f, "----------------------------------------------------------------------\n");
      
      nowstruct = localtime( &(ompp_tstart.tv_sec) );
      strftime( nowstr, 128, "%a %b %d %T %Y", nowstruct );
      fprintf( f, "Start Date      : %s\n", nowstr);

      nowstruct = localtime( &(ompp_tstop.tv_sec) );
      strftime( nowstr, 128, "%a %b %d %T %Y", nowstruct );
      fprintf( f, "End Date        : %s\n", nowstr);

      fprintf( f, "Duration        : %.2f sec\n", ompp_duration );
      fprintf( f, "Application Name: %s\n", ompp_executable_name);

      fprintf( f, "Type of Report  : %s\n", ompp_done?"final":"incremental");
      fprintf( f, "User Time       : %.2f sec\n", ompp_usertime );
      fprintf( f, "System Time     : %.2f sec\n", ompp_systemtime );
      fprintf( f, "Max Threads     : %d\n", ompp_max_threads );

      fprintf( f, "ompP Version    : %u.%u.%u\n",
	       OMPP_VERSION_MAJOR,
	       OMPP_VERSION_MINOR,
	       OMPP_VERSION_REVISION );
      fprintf( f, "ompP Build Date : %s\n", OMPP_BUILD );


#ifdef OMPP_USE_PAPI
      fprintf( f, "PAPI Support    : available\n");
      fprintf( f, "Max Counters    : %d\n", OMPP_PAPI_MAX_CTRS);
      fprintf( f, "PAPI Active     : %s\n", (OMPP_IS_PAPI_ON?"yes":"no") );
      fprintf( f, "Used Counters   : %d\n", nctrs );
      for( i=0; i<OMPP_PAPI_MAX_CTRS; i++ )
	{
	  fprintf( f, "OMPP_CTR%-2d      : %s\n", i+1,
		   (ompp_papi_ctr_name[i]?ompp_papi_ctr_name[i]:"not set") ); 
	}
      fprintf( f, "Max Evaluators  : %d\n", OMPP_MAX_EVALUATORS );
      fprintf( f, "Used Evaluators : %d\n", nevals);
      for( i=0; i<OMPP_MAX_EVALUATORS; i++ )
	{
	  fprintf( f, "OMPP_EVAL%-2d     : %s\n", i+1,
		   (ompp_eval_string[i]?ompp_eval_string[i]:"not set") ); 
	}

#ifdef OMPP_DATARANGE

      if( ompp_papi_daddr ) 
	{
	  fprintf( f, "Data Address    : %p\n", ompp_papi_daddr );
	  fprintf( f, "Data Size       : %lld\n", ompp_papi_dsize );
	  if( ompp_papi_dname )
	    fprintf( f, "Data Name       : %s\n", ompp_papi_dname );
	  else
	    fprintf( f, "Data Name       : not set\n" );
	  fprintf( f, "Data Start Offs.: %lld\n", ompp_papi_doffs );
	  fprintf( f, "Data End   Offs.: %lld\n", ompp_papi_doffe );
	}
      else
	{
	  fprintf( f, "Data Address    : not set\n" );
	  fprintf( f, "Data Size       : N/A\n" );
	  fprintf( f, "Data Name       : N/A\n" );
	}
#endif 

#else
      fprintf( f, "PAPI Support    : not available\n");
#endif

	
	/*
	  fprintf( f, "OMP_NUM_THREADS : %s\n", getenv("OMP_NUM_THREADS"));
	  fprintf( f, "OMP_SCHEDULE    : %s\n", getenv("OMP_SCHEDULE"));
	  fprintf( f, "OMP_DYNAMIC     : %s\n", getenv("OMP_DYNAMIC"));
	  fprintf( f, "OMP_NESTED      : %s\n", getenv("OMP_NESTED"));
	*/
	
	break;
	
      case OMPP_OUTFORMAT_CSV:
	fprintf( f, "##BEG header separator=%c\n", ompp_sep );

      nowstruct = localtime( &(ompp_tstart.tv_sec) );
      strftime( nowstr, 128, "%a %b %d %T %Y", nowstruct );
      fprintf( f, "Start Date%c%s\n", ompp_sep, nowstr);

      nowstruct = localtime( &(ompp_tstop.tv_sec) );
      strftime( nowstr, 128, "%a %b %d %T %Y", nowstruct );
      fprintf( f, "End Date%c%s\n", ompp_sep, nowstr);
      
      fprintf( f, "Duration%c%f%csec\n", 
	       ompp_sep, ompp_duration, ompp_sep );
      fprintf( f, "Application Name%c%s\n", ompp_sep, 
	       ompp_executable_name);
      fprintf( f, "Type of Report%c%s\n", ompp_sep,
	       ompp_done?"final":"incremental");

      fprintf( f, "Thread Count%c%d\n", ompp_sep, ompp_max_threads );

      fprintf( f, "ompP Version Major%c%u\n",    ompp_sep, OMPP_VERSION_MAJOR );
      fprintf( f, "ompP Version Minor%c%u\n",    ompp_sep, OMPP_VERSION_MINOR );
      fprintf( f, "ompP Version Revision%c%u\n", ompp_sep, OMPP_VERSION_REVISION );
      fprintf( f, "ompP Build Date%c%s\n", ompp_sep, OMPP_BUILD );

#ifdef OMPP_USE_PAPI
      fprintf( f, "PAPI Support%cavailable\n", 
	       ompp_sep);
      fprintf( f, "Max Counters%c%d\n", 
	       ompp_sep, OMPP_PAPI_MAX_CTRS);
      fprintf( f, "PAPI Active%c%s\n", 
	       ompp_sep, (OMPP_IS_PAPI_ON?"yes":"no") );
      fprintf( f, "Used Counters%c%d\n", 
	       ompp_sep, nctrs );

      for( i=0; i<OMPP_PAPI_MAX_CTRS; i++ )
      {
	fprintf( f, "OMPP_CTR%d%c%s\n", i+1, ompp_sep, 
		   (ompp_papi_ctr_name[i]?ompp_papi_ctr_name[i]:"not set") ); 
      }

      fprintf( f, "Max Evaluators%c%d\n", 
	       ompp_sep, OMPP_MAX_EVALUATORS );
      fprintf( f, "Used Evaluators%c%d\n", 
	       ompp_sep, nevals);
      for( i=0; i<OMPP_MAX_EVALUATORS; i++ )
	{
	  fprintf( f, "OMPP_EVAL%d%c%s\n", i+1, ompp_sep, 
                   (ompp_eval_string[i]?ompp_eval_string[i]:"not set") );
        }
#else
      fprintf( f, "PAPI Support%cnot available\n", ompp_sep);
#endif

      break;
      }

#ifdef OMPP_MPI
  {
    int i;
    int bout=0; int nsend=0;
    int bin=0; int nrecv=0;
    int ncoll=0;
    double time=0.0;
    
    for( i=0; i<ompp_max_threads; i++ )
      {
	nsend+=ompp_mpi_nsend[i];
	nrecv+=ompp_mpi_nrecv[i];
	ncoll+=ompp_mpi_ncoll[i];
	bout+=ompp_mpi_bout[i];
	bin+=ompp_mpi_bin[i];
	time +=ompp_mpi_time[i];
      }
    
    switch( ompp_outformat )
      {
      case OMPP_OUTFORMAT_PLAIN:
	fprintf( f, "MPI my rank     : %d\n", ompp_mpi_myrank );
	fprintf( f, "MPI num procs   : %d\n", ompp_mpi_nprocs );
	fprintf( f, "MPI hostname    : %s\n", ompp_hostname );
	fprintf( f, "MPI time        : %f\n", time );
	fprintf( f, "MPI bytes in    : %d\n", bin );
	fprintf( f, "MPI bytes out   : %d\n", bout );
	fprintf( f, "MPI recv calls  : %d\n", nrecv );
	fprintf( f, "MPI send calls  : %d\n", nsend );
	fprintf( f, "MPI collectives : %d\n", ncoll );
	break;
      case OMPP_OUTFORMAT_CSV:
	fprintf( f, "MPI my rank%c%d\n", ompp_sep, ompp_mpi_myrank );
	fprintf( f, "MPI num procs%c%d\n", ompp_sep, ompp_mpi_nprocs );
	fprintf( f, "MPI hostname%c%s\n", ompp_sep, ompp_hostname );
	fprintf( f, "MPI time%c%f\n", ompp_sep, time );
	fprintf( f, "MPI bytes in%c%d\n", ompp_sep, bin );
	fprintf( f, "MPI bytes out%c%d\n", ompp_sep, bout );
	fprintf( f, "MPI recv calls%c%d\n", ompp_sep,  nrecv );
	fprintf( f, "MPI send calls%c%d\n", ompp_sep, nsend );
	fprintf( f, "MPI collectives%c%d\n", ompp_sep, ncoll );
	break;
      }
  }
#endif
  
  if( ompp_outformat==OMPP_OUTFORMAT_CSV )
    fprintf( f, "##END header\n" );
  else
    fprintf(f, "\n");
  
}

void dump_regstats( FILE *f, dllist_t *list )
{
  lelem_t *elem; 
  ompp_region_t *reg;

  elem=list->head;
  while( elem )
    {
      reg = (ompp_region_t*)elem->data;
      
      regstat_dump( reg->rstat, f );

      elem=elem->next;
    }
}

void print_region_stats( FILE *f, dllist_t *list )
{
  lelem_t *elem; 
  ompp_region_t *reg;

  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "----------------------------------------------------------------------\n");
      fprintf(f, "----     ompP Flat Region Profile (inclusive data)     ---------------\n");
      fprintf(f, "----------------------------------------------------------------------\n");
      break;

    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##BEG flat region profiles\n");
      break;
    }
      
  elem=list->head;
  while( elem )
    {
      reg = (ompp_region_t*)elem->data;

      if( ompp_outformat==OMPP_OUTFORMAT_CSV )
	fprintf(f, "##BEG R%05u flat profile\n", reg->number);
      
      regstat_print_header( reg->rstat, f );
      regstat_print( f, reg->rstat, 0 );

      
      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  fprintf(f, "\n");
	  break;
	  
	case OMPP_OUTFORMAT_CSV:
	  fprintf(f, "##END R%05u flat profile\n", reg->number);
	  fprintf(f, "\n");
	  break;
	}
      elem=elem->next;
    }

  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      break;

    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##END flat region profiles\n");
      fprintf(f, "\n");
      break;
    }



}


void print_region_list( FILE *f, dllist_t *list )
{
  int i, j;
  lelem_t *elem; 
  ompp_region_t *reg;

  typedef struct reginfo
  {
    int             count; // number of regions
    ompp_region_t **regions;
  }
  reginfo_t;

  reginfo_t reginfo[OMPP_LAST];

  for( i=0; i<OMPP_LAST; i++ )
    {
      reginfo[i].count=0;
      reginfo[i].regions=0;
    }
  
  // count the regions
  elem=list->head;
  while( elem )
    {
      reg = (ompp_region_t*)elem->data;

      reginfo[reg->type].count++;
      elem=elem->next;
    }

  for( i=0; i<OMPP_LAST; i++ )
    {
      reginfo[i].regions=(ompp_region_t**)
	malloc(reginfo[i].count * sizeof(ompp_region_t*));

      // we can now use count as an indicator where to 
      // put the next region_t
      reginfo[i].count=0;
    }

  elem=list->head;
  while( elem )
    {
      reg = (ompp_region_t*)elem->data;

      i=reginfo[reg->type].count;
      reginfo[reg->type].regions[i]=reg;
      reginfo[reg->type].count++;

      elem=elem->next;
    }


  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "----------------------------------------------------------------------\n");
      fprintf(f, "----     ompP Region Overview     ------------------------------------\n");
      fprintf(f, "----------------------------------------------------------------------\n");
      break;

    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##BEG region overview\n");
      break;
    }

  
  for( i=0; i<OMPP_LAST; i++ )
    {
      if( reginfo[i].count>0 )
	{
	  const char *regstr;
	  if( reginfo[i].count==1 )
	    regstr="region";
	  else
	    regstr="regions";
	  
	  switch( ompp_outformat )
	    {
	    case OMPP_OUTFORMAT_PLAIN:
	      fprintf(f, "%s: %d %s:\n", ompp_region_full_name[i],
		      reginfo[i].count, regstr);
	      break;
	    case OMPP_OUTFORMAT_CSV:
	      fprintf(f, "%s%c%d\n",  
		      ompp_region_full_name[i], ompp_sep,
		      reginfo[i].count );
	      break;
	    }
	  
	  for( j=0; j<reginfo[i].count; j++ )
	    {
	      reg = reginfo[i].regions[j];
	      assert(reg);
	  
	      switch( ompp_outformat )
		{
		case OMPP_OUTFORMAT_PLAIN:
		  fprintf(f, " * R%05u %s\n", reg->number, reg->location);
		  break;
		case OMPP_OUTFORMAT_CSV:
		  fprintf(f, "R%05u%c%s%c%s\n",
			  reg->number, ompp_sep,
			  ompp_region_full_name[i], ompp_sep,
			  reg->csvloc);
		  break;
		}
	    }
	  
	  switch( ompp_outformat )
	    {
	    case OMPP_OUTFORMAT_PLAIN:
	      fprintf(f, "\n");
	      break;
	    case OMPP_OUTFORMAT_CSV:
	      break;
	    }
	}
    }


  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      break;

    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##END region overview\n");
      fprintf(f, "\n");
      break;
    }
  
}


void print_overhead_report( FILE *f, 
			    overheads_t *ovhds, unsigned npara )
{
  int i, j;
  double wallt, parwallt;
  double total;
  double sum_sync, sum_imbal, sum_mgmt, sum_limpar, sum_all;
  char buf[40];

#ifdef OMPP_MPI
  double sum_mpit;
#endif 

#ifdef OMPP_USE_PAPI
  long long ctrsum[OMPP_PAPI_MAX_CTRS];
  double dctr[OMPP_PAPI_MAX_CTRS];
  double res;
#endif

  // wallclock-runtime 
  wallt = ompp_duration;

  // parallel wallclock-runtime
  parwallt=0.0;

#ifdef OMPP_USE_PAPI
  for( j=0; j<OMPP_PAPI_MAX_CTRS; j++ )
  {
      ctrsum[j]=0;
  }

  for( i=0; i<npara; i++ )
  {
      parwallt+=ovhds[i].wallt;

      for( j=0; j<OMPP_PAPI_MAX_CTRS; j++ )
      {
	  ctrsum[j]+=ovhds[i].ctr[j];
      }
  }
#else
  for( i=0; i<npara; i++ )
  {
      parwallt+=ovhds[i].wallt;
  }
#endif

    
  //
  // H E A D E R
  //

  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "----------------------------------------------------------------------\n");
      fprintf(f, "----     ompP Overhead Analysis Report     ---------------------------\n");
      fprintf(f, "----------------------------------------------------------------------\n");
      
      fprintf(f, "Total runtime (wallclock)   : %.2f sec [%d thread",
	      wallt, ompp_max_threads);
      fprintf(f, (ompp_max_threads>1?"s]\n":"]\n"));
      
      fprintf(f, "Number of parallel regions  : %u\n", npara);
      
      fprintf(f, "Parallel coverage           : %.2f sec (%5.2f%%)\n",
	      parwallt, parwallt/wallt*100.0);
      
      fprintf(f, "\n");
      break;

    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##BEG overhead analysis report\n");
      
      fprintf(f, "Total runtime (wallclock) [secs]%c%f\n", 
	      ompp_sep, wallt );
      fprintf(f, "Number of threads%c%d\n",
	      ompp_sep, ompp_max_threads );
      fprintf(f, "Number of parallel regions%c%u\n",
	      ompp_sep, npara );
      fprintf(f, "Parallel coverage [secs]%c%f\n", 
	      ompp_sep, parwallt );
      fprintf(f, "Parallel coverage [percent]%c%f\n",
	      ompp_sep, parwallt/wallt*100.0 );
      
      fprintf(f, "\n");
      break;
    }


  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "Parallel regions sorted by wallclock time:\n");
      break;
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##BEG parallel regions sorted by wallclock time\n");
      break;
    }
      
  qsort( ovhds, npara, sizeof(overheads_t), ovhd_compare_by_wallclock );

  if( ompp_outformat == OMPP_OUTFORMAT_PLAIN )
    {
      fprintf(f, "            Type                            Location      Wallclock (%%) ");
#ifdef OMPP_USE_PAPI
      for( j=0; j<OMPP_PAPI_MAX_CTRS; j++ )
	{
	  if( ompp_papi_ctr_name[j] )
	    {
	      fprintf(f, " %18s", ompp_papi_ctr_name[j]);
	    }
	}

      for( j=0; j<OMPP_MAX_EVALUATORS; j++ )
	{
	  if( ompp_eval_string[j] )
	    {
	      sprintf(buf, "OMPP_EVAL%-2d", j+1);
	      fprintf(f, " %13s", buf);
	    }
	}
#endif
      fprintf(f, "\n");
    } 
  

  if( ompp_outformat == OMPP_OUTFORMAT_CSV )
    {
      fprintf(f, "%cType%cWallclock%c(%%)", 
	      ompp_sep, ompp_sep, ompp_sep );
#ifdef OMPP_USE_PAPI
      for( j=0; j<OMPP_PAPI_MAX_CTRS; j++ )
	{
	  if( ompp_papi_ctr_name[j] )
	    {
	      fprintf(f, "%c%s", ompp_sep, 
		      ompp_papi_ctr_name[j] );
	    }
	}

      for( j=0; j<OMPP_MAX_EVALUATORS; j++ )
	{
	  if( ompp_eval_string[j] )
	    {
	      sprintf(buf, "OMPP_EVAL%-2d", j+1);
	      fprintf(f, "%c%s", ompp_sep, buf);
	    }
	}
#endif
      fprintf(f, "\n");
    }

  
  if( ompp_outformat == OMPP_OUTFORMAT_PLAIN )
    {
      for( i=0; i<npara; i++ )
	{
	  fprintf(f, "R%05u  %8s %35s    %7.2f (%5.2f) ", 
		  ovhds[i].reg->number,
		  ompp_region_name[ovhds[i].reg->type],
		  ovhds[i].reg->location,
		  ovhds[i].wallt,
		  100.0*ovhds[i].wallt/wallt);
	  
#ifdef OMPP_USE_PAPI
	  for( j=0; j<OMPP_PAPI_MAX_CTRS; j++ )
	    {
	      if( ompp_papi_ctr_name[j] ) // ctr is enabled
		{
		  lldtostr(buf, ovhds[i].ctr[j]);
		  fprintf(f, " %18s", buf);
		}
	      
	      dctr[j] = (double)(ovhds[i].ctr[j]);
	    }

	  for( j=0; j<OMPP_MAX_EVALUATORS; j++ )
	    {
	      if( ompp_eval_string[j] )
		{
		  res = evaluator_evaluate( ompp_eval_evaluator[j],
					    OMPP_PAPI_MAX_CTRS,
					    ompp_papi_ctr_eval_name,
					    dctr );
		  
		  fprintf(f, " %12.2f ", res  );
		}
	    }
#endif 
	  fprintf(f, "\n");
	}
	
	fprintf(f, "               %s %35s    %7.2f (%5.2f) ",
		" ",
		"SUM",
		parwallt, 100.0*parwallt/wallt );
	
#ifdef OMPP_USE_PAPI
	for( j=0; j<OMPP_PAPI_MAX_CTRS; j++ )
	  {
	    if( ompp_papi_ctr_name[j] ) // ctr is enabled
	      {
		lldtostr(buf, ctrsum[j]);
		fprintf(f, " %18s", buf);
	      }
	    
	    dctr[j] = ctrsum[j];
	  }
	
	for( j=0; j<OMPP_MAX_EVALUATORS; j++ )
	  {
	    if( ompp_eval_string[j] )
	      {
		res = evaluator_evaluate( ompp_eval_evaluator[j],
					  OMPP_PAPI_MAX_CTRS,
					  ompp_papi_ctr_eval_name,
					  dctr );
		
		fprintf(f, " %12.2f ", res  );
	      }
	  }
#endif
	fprintf(f, "\n");    
      }

  if( ompp_outformat == OMPP_OUTFORMAT_CSV )
    {
      for( i=0; i<npara; i++ )
	{
	  fprintf(f, "R%05u%c%s%c%f%c%f", 
		  ovhds[i].reg->number, ompp_sep, 
		  ompp_region_name[ovhds[i].reg->type], ompp_sep, 
		  ovhds[i].wallt, ompp_sep, 
		  100.0*ovhds[i].wallt/wallt);
	  
#ifdef OMPP_USE_PAPI
	  for( j=0; j<OMPP_PAPI_MAX_CTRS; j++ )
	    {
	      if( ompp_papi_ctr_name[j] )  // ctr is enabled
		{
		  fprintf(f, "%c%lld", ompp_sep, ovhds[i].ctr[j]);
		}
	      dctr[j] = (double)(ovhds[i].ctr[j]);
	    }
	  
	  for( j=0; j<OMPP_MAX_EVALUATORS; j++ )
	    {
	      if( ompp_eval_string[j] )
		{
		  res = evaluator_evaluate( ompp_eval_evaluator[j],
					    OMPP_PAPI_MAX_CTRS,
					    ompp_papi_ctr_eval_name,
					    dctr );
		  
		  fprintf(f, "%c%f ", ompp_sep, res  );
		}
	    }
#endif 
	  fprintf(f, "\n");
	}
	
      fprintf(f, "SUM%c%f%c%c%f",
	      ompp_sep, parwallt, ompp_sep, ompp_sep,
	      100.0*parwallt/wallt );
      
#ifdef OMPP_USE_PAPI
      for( j=0; j<OMPP_PAPI_MAX_CTRS; j++ )
	  {
	    if( ompp_papi_ctr_name[j] ) // ctr is enabled
	      {
		fprintf(f, "%c%lld", ompp_sep, ctrsum[j]);
	      }
	    dctr[j] = (double)(ctrsum[j]);
	  }

      for( j=0; j<OMPP_MAX_EVALUATORS; j++ )
	{
	  if( ompp_eval_string[j] )
	    {
	      res = evaluator_evaluate( ompp_eval_evaluator[j],
					OMPP_PAPI_MAX_CTRS,
					ompp_papi_ctr_eval_name,
					dctr );
	      
		  fprintf(f, "%c%f ", ompp_sep, res  );
	    }
	}
#endif 
	fprintf(f, "\n");          
    }
  

  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "\n");
      break;
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##END parallel regions sorted by wallclock time\n");
      fprintf(f, "\n");
      break;
    }

  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "Overheads wrt. each individual parallel region:\n");
      break;
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##BEG overheads for parallel region\n");
      break;
    }

  
  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "          Total        Ovhds (%%)  =   Synch  (%%)  +  Imbal   (%%)  +   Limpar (%%)   +    Mgmt (%%)");
#ifdef OMPP_MPI
      fprintf(f, "   +     MPI (%%)");
#endif
      fprintf(f, "\n");
      
      break;
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "%cTotal%cOvhds%c(%%)%cSynch%c(%%)%cImbal%c(%%)%cLimpar%c(%%)%cMgmt%c(%%)",
	      ompp_sep, ompp_sep, ompp_sep, ompp_sep, ompp_sep, ompp_sep, 
	      ompp_sep, ompp_sep, ompp_sep, ompp_sep, ompp_sep );
#ifdef OMPP_MPI
      fprintf(f, "%cMPI%c(%%)", ompp_sep, ompp_sep);
#endif
      fprintf(f, "\n");

      break;
    }
  
  for( i=0; i<npara; i++ )
    {
      unsigned nthreads;
      
      nthreads = ovhds[i].reg->rstat->nthreads;
      total = ovhds[i].wallt*(double)nthreads;
      
      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  fprintf(f, "R%05u  %7.2f  %7.2f (%5.2f) %7.2f (%5.2f) %7.2f (%5.2f) %7.2f (%5.2f) %7.2f (%5.2f)",
		  ovhds[i].reg->number,
		  total,
		  ovhds[i].sumovhd, 100.0*ovhds[i].sumovhd/total,
		  ovhds[i].sync   , 100.0*ovhds[i].sync/total,
		  ovhds[i].imbal  , 100.0*ovhds[i].imbal/total,
		  ovhds[i].limpar , 100.0*ovhds[i].limpar/total,
		  ovhds[i].mgmt   , 100.0*ovhds[i].mgmt/total);
#ifdef OMPP_MPI
	  fprintf(f, " %7.2f (%5.2f)", 
		  ovhds[i].mpit   , 100.0*ovhds[i].mpit/total);
#endif
	  fprintf(f, "\n");
	  break;
	  
	case OMPP_OUTFORMAT_CSV:
	  fprintf(f, "R%05u%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f",
		  ovhds[i].reg->number, ompp_sep,
		  total, ompp_sep,
		  ovhds[i].sumovhd, ompp_sep, 100.0*ovhds[i].sumovhd/total, ompp_sep,
		  ovhds[i].sync   , ompp_sep, 100.0*ovhds[i].sync/total, ompp_sep,
		  ovhds[i].imbal  , ompp_sep, 100.0*ovhds[i].imbal/total, ompp_sep,
		  ovhds[i].limpar , ompp_sep, 100.0*ovhds[i].limpar/total, ompp_sep,
		  ovhds[i].mgmt   , ompp_sep, 100.0*ovhds[i].mgmt/total );
#ifdef OMPP_MPI
	  fprintf(f, "%c%f%c%f", ompp_sep,
		  ovhds[i].mpit, ompp_sep, 
		  100.0*ovhds[i].mpit/total);
#endif
	  fprintf(f, "\n");
	  break;
	}
    }
  

  // re-sort
  qsort( ovhds, npara, sizeof(overheads_t), ovhd_compare_by_overheads );

  sum_sync = sum_imbal = sum_mgmt = sum_limpar = sum_all=0.0;
  
#ifdef OMPP_MPI
  sum_mpit=0.0;
#endif


  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "\n");
      break;
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##END overheads for parallel region\n");
      fprintf(f, "\n");
      break;
    }
  

  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "Overheads wrt. whole program:\n");
      break;
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##BEG overheads for whole program\n");
      break;
    }

  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "          Total        Ovhds (%%)  =   Synch  (%%)  +  Imbal   (%%)  +   Limpar (%%)   +    Mgmt (%%)");
#ifdef OMPP_MPI
      fprintf(f, "   +     MPI (%%)");
#endif
      fprintf(f, "\n");
      break;
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "%cTotal%cOvhds%c(%%)%cSynch%c(%%)%cImbal%c(%%)%cLimpar%c(%%)%cMgmt%c(%%)",
	      ompp_sep, ompp_sep, ompp_sep, ompp_sep, ompp_sep, ompp_sep, 
	      ompp_sep, ompp_sep, ompp_sep, ompp_sep, ompp_sep );

#ifdef OMPP_MPI
      fprintf(f, "%cMPI%c(%%)", ompp_sep, ompp_sep);
#endif
      fprintf(f, "\n");
      break;
    }
  
  for( i=0; i<npara; i++ )
  {
      unsigned nthreads;
      
      nthreads = ovhds[i].reg->rstat->nthreads;
      total = wallt*(double)nthreads;

      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  fprintf(f, "R%05u  %7.2f  %7.2f (%5.2f) %7.2f (%5.2f) %7.2f (%5.2f) %7.2f (%5.2f) %7.2f (%5.2f)",
		  ovhds[i].reg->number,
		  ovhds[i].wallt*(double)nthreads,
		  ovhds[i].sumovhd, 100.0*ovhds[i].sumovhd/total,
		  ovhds[i].sync   , 100.0*ovhds[i].sync/total,
		  ovhds[i].imbal  , 100.0*ovhds[i].imbal/total,
		  ovhds[i].limpar , 100.0*ovhds[i].limpar/total,
		  ovhds[i].mgmt   , 100.0*ovhds[i].mgmt/total);
#ifdef OMPP_MPI
	  fprintf(f, " %7.2f (%5.2f)", 
		  ovhds[i].mpit   , 100.0*ovhds[i].mpit/total);
#endif
	  fprintf(f, "\n");
	  
	  break;
	case OMPP_OUTFORMAT_CSV:
	  fprintf(f, "R%05u%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f",
		  ovhds[i].reg->number, ompp_sep, 
		  ovhds[i].wallt*(double)nthreads, ompp_sep,
		  ovhds[i].sumovhd, ompp_sep, 100.0*ovhds[i].sumovhd/total, ompp_sep,
		  ovhds[i].sync   , ompp_sep, 100.0*ovhds[i].sync/total, ompp_sep,
		  ovhds[i].imbal  , ompp_sep, 100.0*ovhds[i].imbal/total, ompp_sep,
		  ovhds[i].limpar , ompp_sep, 100.0*ovhds[i].limpar/total, ompp_sep,
		  ovhds[i].mgmt   , ompp_sep, 100.0*ovhds[i].mgmt/total );
#ifdef OMPP_MPI
	  fprintf(f, "%c%f%c%f", ompp_sep, 
		  ovhds[i].mpit, ompp_sep, 
		  100.0*ovhds[i].mpit/total);
#endif
	  fprintf(f, "\n");

	  break;
	}

      sum_sync  +=ovhds[i].sync;
      sum_imbal +=ovhds[i].imbal;
      sum_limpar+=ovhds[i].limpar;
      sum_mgmt  +=ovhds[i].mgmt;
      
#ifdef OMPP_MPI
      sum_mpit  +=ovhds[i].mpit;
#endif 

      sum_all   +=ovhds[i].sumovhd;
  }

  total = wallt*(double)ompp_max_threads;

  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "   SUM  %7.2f  %7.2f (%5.2f) %7.2f (%5.2f) %7.2f (%5.2f) %7.2f (%5.2f) %7.2f (%5.2f)",
          parwallt * ompp_max_threads,
	      sum_all   , 100.0*sum_all/total,
	      sum_sync  , 100.0*sum_sync/total,
	      sum_imbal , 100.0*sum_imbal/total,
	      sum_limpar, 100.0*sum_limpar/total,
	      sum_mgmt  , 100.0*sum_mgmt/total);
#ifdef OMPP_MPI
      fprintf(f, " %7.2f (%5.2f)",
	      sum_mpit  , 100.0*sum_mpit/total);
#endif
      fprintf(f, "\n");
	      
      break;
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "SUM%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f",
	      ompp_sep,
	      parwallt * ompp_max_threads, ompp_sep,
	      sum_all   , ompp_sep, 100.0*sum_all/total, ompp_sep,
	      sum_sync  , ompp_sep, 100.0*sum_sync/total, ompp_sep,
	      sum_imbal , ompp_sep, 100.0*sum_imbal/total, ompp_sep,
	      sum_limpar, ompp_sep, 100.0*sum_limpar/total, ompp_sep,
	      sum_mgmt  , ompp_sep, 100.0*sum_mgmt/total);
#ifdef OMPP_MPI
      fprintf(f, "%c%f%c%f", ompp_sep,
	      sum_mpit  , ompp_sep, 100.0*sum_mpit/total);
#endif
      fprintf(f,"\n");
      break;
    }

  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "\n");
      break;
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##END overheads for whole program\n");
      fprintf(f, "\n");
      break;
    }

  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "\n");
      break;

    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##END overhead analysis report\n");
      fprintf(f, "\n");
      break;
    }



}


void print_properties_report( FILE *f,
			      ompp_perfprop_t *props, unsigned nprop )
{
  unsigned i;
  double sev;

  qsort( props, nprop, sizeof(ompp_perfprop_t), 
	 perfprop_compare_by_severtiy );

  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "----------------------------------------------------------------------\n");
      fprintf(f, "----     ompP Performance Properties Report     ----------------------\n");
      fprintf(f, "----------------------------------------------------------------------\n");
      fprintf(f, "\n");
      break;

    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##BEG ompp performance properties report\n");
      break;
    }

  
  for( i=0; i<nprop; i++ )
    {
      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  fprintf(f, "Property P%05u '%s' holds for\n", (i+1),		  
		  ompp_perfprop_name[props[i].type]);
	  fprintf(f, "     '%s', with a severity (in percent) of %f\n",
		  props[i].location, (100.0*props[i].severity) );
	  fprintf(f, "\n");
	  break;
	  
	case OMPP_OUTFORMAT_CSV:
	  fprintf(f, "%s%c%s%c%f\n",
		  ompp_perfprop_name[props[i].type], ompp_sep,
		  props[i].location, ompp_sep,
		  props[i].severity );
	  break;
	}
    }

  switch( ompp_outformat )
    {
      break;
      
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##END ompp performance properties report\n");
      break;
    }

}

