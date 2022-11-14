/* 
 * ompp_pomp.c  */
/* @OMPP_HEADER@ */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>

#include "ompp.h"
#include "ompp_pomp.h"
#include "ompp_region.h"
#include "ompp_regstat.h"
#include "pomp_lib.h"
#include "utils/hashtab.h"

//
// this lock is used to prevent race conditions
// when multiple threads try to create an ompp_region for
// the same region descriptor
//
omp_lock_t make_region_lock;

omp_lock_t dump_lock;


//
// used to map OpenMP locks to ompP regions
//
struct hashtab *pomp_locks;



//
// Additional initialization of the POMP part, this function 
// is called from the overall initialization routine
//
void ompp_pomp_initialize()
{
  omp_init_lock( &make_region_lock );

  omp_init_lock( &dump_lock );

  // XXX: optimal size of this hash table?
  pomp_locks = hashtab_create( ompp_lockhash_size );

  
}


void ompp_pomp_region_enter( struct ompregdescr *r,
			     ompp_reg_type_t type )
{
  ompp_region_t *reg;
  int tid;

  assert(r);

  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }

  if( !(OMPP_IS_MONITOR_ON) )
    {
      return;
    }

  reg=ompp_get_region_for_descr( r, type );
  assert(reg);

  /* tid=omp_get_thread_num(); */
  OMPP_GET_THREAD_NUM(tid);


  if( type==OMPP_TASKEXEC||type==OMPP_UTASKEXEC )
    {
      if( ompp_current_task[tid]!=0 &&
	  ompp_current_task[tid]!=reg )
	{
	  //fprintf(stderr, "** thread %d: taskswitching has occured!\n",
	  //tid);

	  // switch out the old region
	  // ompp_region_enter( ompp_current_task[tid], OMPP_TASKSWITCH, tid );
	  // reg->switched_out[tid]=ompp_current_task[tid];
	}
      
      ompp_current_task[tid]=reg;
    }
  
  ompp_region_enter( reg, type, tid );
}


void ompp_pomp_region_exit( struct ompregdescr *r,
			    ompp_reg_type_t type )
{
  ompp_region_t *reg;
  int tid;

  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }

  if( !(OMPP_IS_MONITOR_ON) )
    {
      return;
    }

  reg = ompp_get_region_for_descr( r, type );
  assert(reg);

  /* tid = omp_get_thread_num(); */
  OMPP_GET_THREAD_NUM(tid);

  if( type==OMPP_TASKEXEC||type==OMPP_UTASKEXEC )
    {
      ompp_current_task[tid]=0;
    }

  ompp_region_exit( reg, type, tid );
  
}



void ompp_pomp_init_lock(omp_lock_t *lock)
{
  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }

  //
  // this call is just passed through, but the 
  // ompp_get_region_for_lock() call will initialize the
  // mapping from lock pointer to ompp region 
  //
  if( ompp_construct_enabled[OMPP_LOCK] )
    ompp_get_region_for_lock(lock);
  
  omp_init_lock(lock);
}


void ompp_pomp_destroy_lock(omp_lock_t *lock)
{
  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }

  //
  // as above, the call is just passed through,
  // but initization will happen
  //
  if( ompp_construct_enabled[OMPP_LOCK] )
    ompp_get_region_for_lock(lock);
  
  omp_destroy_lock(lock);
}



void ompp_pomp_set_lock(omp_lock_t *lock)
{
  ompp_region_t *reg;
  int tid;
  
  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }

  //
  // either determine actual thread number or, if 
  // ompp_collapse_locks is present, ignore thread number
  // and use 0 instead
  //
  if( ompp_collapse_locks )
    {
      tid=0;
    }
  else
    {
      /* tid=omp_get_thread_num(); */
      OMPP_GET_THREAD_NUM(tid);
    }


  if( ompp_construct_enabled[OMPP_LOCK] )
    {
      reg = ompp_get_region_for_lock(lock);
      assert(reg);

      ompp_region_enter( reg, OMPP_LOCK, tid );
      omp_set_lock(lock);
      ompp_region_enter( reg, OMPP_LOCK_BODY, tid );
    }
  else
    {
      omp_set_lock(lock);
    }
}


void ompp_pomp_unset_lock(omp_lock_t *lock)
{
  ompp_region_t *reg;
  int tid;

  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }

  //
  // either determine actual thread number or, if 
  // ompp_collapse_locks is present, ignore thread number
  // and use 0 instead
  //
  if( ompp_collapse_locks ) 
    {
      tid=0;
    }
  else
    {
      /* tid=omp_get_thread_num(); */
      OMPP_GET_THREAD_NUM(tid);
    }


  if( ompp_construct_enabled[OMPP_LOCK] )  
    {
      reg = ompp_get_region_for_lock(lock);
      assert(reg);

      ompp_region_exit( reg, OMPP_LOCK_BODY, tid );
      omp_unset_lock(lock);
      ompp_region_exit( reg, OMPP_LOCK, tid );
    }
  else
    {
      omp_unset_lock(lock);
    }
  
}


int ompp_pomp_test_lock( omp_lock_t *lock )
{
  ompp_region_t *reg;
  int result;
  int tid;

  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }

  //
  // either determine actual thread number or, if 
  // ompp_collapse_locks is present, ignore thread number
  // and use 0 instead
  //
  if( ompp_collapse_locks )
    {
      tid=0;
    }
  else
    {
      /* tid=omp_get_thread_num(); */
      OMPP_GET_THREAD_NUM(tid);
    }


  if( ompp_construct_enabled[OMPP_LOCK] )  
    {
      reg = ompp_get_region_for_lock(lock);
      assert(reg);

      ompp_region_enter( reg, OMPP_LOCK, tid );
      result = omp_test_lock(lock);
      if( result )
	{
	  ompp_region_enter( reg, OMPP_LOCK_BODY, tid );
	}
      else
	{
	  ompp_region_exit( reg, OMPP_LOCK_FAIL, tid );
	}
    }
  else
    {
      result = omp_test_lock(lock);
    }
  return result;
}





//
// get an ompp_region for the specified ompregdescr,
// a new ompp_region is created if required
//
ompp_region_t* 
ompp_get_region_for_descr( struct ompregdescr *r,
			   ompp_reg_type_t type )
{
  ompp_region_t *reg=0;
  char *fname;

  assert(r);

  if( !(reg=(ompp_region_t*)(r->data)) )
    {
      omp_set_lock( &make_region_lock );

      // test again
      if( !(reg=(ompp_region_t*)(r->data)) )
	{
	  // create and initialize new region 
	  reg = (ompp_region_t*) malloc( sizeof(ompp_region_t) );
	  assert(reg);

	  ompp_region_init(reg);
	  reg->desc = r;
	  fname = strdup(r->file_name);
	  fname = basename(fname);

	  reg->number = ompp_region_get_static_number(reg);

	  switch(type)
	    {
	    case OMPP_ATOMIC:
	    case OMPP_BARRIER:
	    case OMPP_CRITICAL:
	    case OMPP_FLUSH:
	    case OMPP_LOOP:
	    case OMPP_MASTER:
	    case OMPP_SECTIONS:
	    case OMPP_SINGLE:
	    case OMPP_WORKSHARE:
	    case OMPP_REGION:
	    case OMPP_TASK:
	    case OMPP_UTASK:
	    case OMPP_TASKWAIT:
	      reg->type = type;
	      break;

	    case OMPP_PARALLEL:
	      // need to differentiata actual type
	      // based on "name" of region
	      reg->type = OMPP_PARALLEL;
	            
	      if( !strcmp( r->name, "parallelfor") ||
		  !strcmp( r->name, "paralleldo") )
		reg->type = OMPP_PARALLEL_LOOP;
	            
	      if( !strcmp( r->name, "parallelsections") )
		reg->type = OMPP_PARALLEL_SECTIONS;
	            
	      if( !strcmp( r->name, "parallelworkshare") )
		reg->type = OMPP_PARALLEL_WORKSHARE;
	      break;
	            
	    default:
	      // TODO: what do we do if another type is
	      // specified??
	      assert(0);
	      break;
	    }
	    
	  reg->rstat = (regstat_t*) malloc( sizeof(regstat_t) );
	  assert(reg->rstat);

	  if( reg->type==OMPP_MASTER )
	    {
	      regstat_init( reg->rstat, reg, 1 );
	    }
	  else
	    {
	      regstat_init( reg->rstat, reg, ompp_max_threads );
	    }

	  switch( reg->type )
	    {
	    case OMPP_FLUSH:
	    case OMPP_BARRIER:
	    case OMPP_TASKWAIT:
	      sprintf( reg->location, "%s (%d)",
		       fname /*r->file_name*/, 
		       r->begin_first_line);

	      sprintf( reg->csvloc, "%s%c%d%c%d%c%s",
		       fname, ompp_sep, 
		       r->begin_first_line, ompp_sep,
		       r->begin_first_line, ompp_sep, "" );
	      break;

	    case OMPP_CRITICAL:
	      if( (r->sub_name) && *(r->sub_name) )
		{
		  // named critical section --> report the name
		  sprintf( reg->location, "%s (%d-%d) ('%s')",
			   fname /* r->file_name*/, 
			   r->begin_first_line, 
			   r->end_first_line, 
			   r->sub_name);

		  sprintf( reg->csvloc, "%s%c%d%c%d%c%s",
			   fname, ompp_sep, 
			   r->begin_first_line, ompp_sep,
			   r->end_first_line, ompp_sep, r->sub_name );

		}
	      else
		{
		  // default critical section
		  sprintf( reg->location, "%s (%d-%d) (unnamed)",
			   fname /*r->file_name*/, 
			   r->begin_first_line, 
			   r->end_first_line);

		  sprintf( reg->csvloc, "%s%c%d%c%d%c%s",
			   fname, ompp_sep, 
			   r->begin_first_line, ompp_sep,
			   r->end_first_line, ompp_sep, "" );
		}
	      break;

	    case OMPP_REGION:
	      if( (r->sub_name) && *(r->sub_name) )
		{
		  sprintf( reg->location, "%s (%d-%d) ('%s')",
			   fname /*r->file_name*/, 
			   r->begin_first_line, 
			   r->end_first_line, 
			   r->sub_name);

		  sprintf( reg->csvloc, "%s%c%d%c%d%c%s",
			   fname, ompp_sep, 
			   r->begin_first_line, ompp_sep,
			   r->end_first_line, ompp_sep, r->sub_name );
		}
	      else
		{
		  sprintf( reg->location, "%s (%d-%d)",
			   fname /*r->file_name*/, 
			   r->begin_first_line, 
			   r->end_first_line);
		  
		  sprintf( reg->csvloc, "%s%c%d%c%d%c%s",
			   fname, ompp_sep, 
			   r->begin_first_line, ompp_sep,
			   r->end_first_line, ompp_sep, "" );
		}


	      break;

	    default:
	      sprintf( reg->location, "%s (%d-%d)",
		       fname /*r->file_name*/, 
		       r->begin_first_line, r->end_first_line);

	      sprintf( reg->csvloc, "%s%c%d%c%d%c%s",
		       fname, ompp_sep, 
		       r->begin_first_line, ompp_sep,
		       r->end_first_line, ompp_sep, "" );
	      
	    }

	  /*
	    if( reg->type==OMPP_PARALLEL || 
	      reg->type==OMPP_PARALLEL_LOOP ||
	      reg->type==OMPP_PARALLEL_SECTIONS ||
	      reg->type==OMPP_PARALLEL_WORKSHARE )
	    {
	      // the region is its own base
	      reg->par_base = reg;
	    }
	  else
	    {
	      // set the current parallel region as the base region
	      reg->par_base = ompp_current_par;
	    }
	  */


	  //
	  // add an element to the region list
	  // 
	  {
	    lelem_t *elem = (lelem_t*) malloc( sizeof(lelem_t) );
	    elem->data = (void*)reg;
	    list_add_back( &ompp_region_list, elem );
	    ompp_region_count[reg->type]++;
	  }


	  /* handle special case of OMPP_TASKS regions */
	  if( reg->type==OMPP_TASK || reg->type==OMPP_UTASK )
	    {
	      ompp_region_t *def=reg;
	      ompp_region_t *exe=(ompp_region_t*) malloc( sizeof(ompp_region_t) );
	      
	      assert(exe);
	      def->sub = exe;
	      
	      ompp_region_init(exe);
	      exe->desc     = def->desc;
	      exe->type     = (reg->type==OMPP_TASK?OMPP_TASKEXEC:OMPP_UTASKEXEC);
	      sprintf( exe->location, "%s", def->location );
	      exe->number   = def->number;
	      
	      exe->rstat = (regstat_t*) malloc( sizeof(regstat_t) );
	      assert(exe->rstat);

	      //fprintf(stderr, "creating subregion\n");
	      regstat_init( exe->rstat, exe, ompp_max_threads );

	      // add an element to the region list
	      {
		lelem_t *elem = (lelem_t*) malloc( sizeof(lelem_t) );
		elem->data = (void*)exe;
		list_add_back( &ompp_region_list, elem );
		ompp_region_count[exe->type]++;
	      }
	  
	    }
	  
	  r->data = reg;
	  
	  omp_unset_lock( &make_region_lock );
	}
      else
	{
	  omp_unset_lock( &make_region_lock );
	}
    }

  assert(reg);

  if( reg->type==OMPP_PARALLEL ||
      reg->type==OMPP_PARALLEL_LOOP ||
      reg->type==OMPP_PARALLEL_SECTIONS ||
      reg->type==OMPP_PARALLEL_WORKSHARE ) 
    {
      //ompp_current_par=reg;
    }

  if( (reg->type==OMPP_TASK && type==OMPP_TASKEXEC) ||
      (reg->type==OMPP_UTASK && type==OMPP_UTASKEXEC) )
    {
      //fprintf(stderr, "handing out subregion\n");
      return reg->sub;
    }
  else
    {
      return reg;
    }
}


//
// get a ompP region for the specified lock, a new region
// is created if it does not already exist
//
ompp_region_t* ompp_get_region_for_lock( omp_lock_t *lock )
{
  ompp_region_t *reg;
  
  reg = (ompp_region_t*) hashtab_search( pomp_locks, lock );

  if( !reg )
    {
      reg = (ompp_region_t*)malloc( sizeof(ompp_region_t) );
      
      ompp_region_init(reg);

      reg->number+=POMP_MAX_ID;
      reg->type = OMPP_LOCK;
      reg->desc = (struct ompregdescr*)lock; // ABUSE!

      
      sprintf( reg->location, "LOCK (%p) %s",
	       lock, (ompp_collapse_locks?"(collapsed stats)":""));

      reg->rstat = (regstat_t*) malloc(sizeof(regstat_t) );
      assert(reg->rstat);

      if( ompp_collapse_locks )
	{
	  regstat_init( reg->rstat, reg, 1 );
	}
      else
	{
	  regstat_init( reg->rstat, reg, ompp_max_threads );
	}

      hashtab_insert( pomp_locks, lock, reg );

      omp_set_lock( &make_region_lock );
      {
        lelem_t *elem = (lelem_t*) malloc( sizeof(lelem_t) );
        elem->data    = (void*)reg;
        list_add_back( &ompp_region_list, elem );
	ompp_region_count[reg->type]++;
      }
      omp_unset_lock( &make_region_lock );
    }
 
  assert(reg);

  /* 
  // XXX base_par does not make much sense for locks ?!
  // XXX fixme
  // if( !(reg->par_base) )
  //   {
  //    fprintf(stderr, "par_base not set, setting to %p\n", ompp_current_par);
  //    reg->par_base = ompp_current_par;
  //   }
  // XXX
  // XXX reg->stat->base_par = 0; 

  if( !(reg->par_base) )
    {
      reg->par_base = ompp_current_par;
    }
  */
  
  return reg;
}



