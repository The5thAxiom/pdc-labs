/* 
 * ompp_fortran.c   */
/* @OMPP_HEADER@ */


#include "pomp_lib.h"
#include "ompp.h"

#include "ompp_papi.h"

/* do not edit those FORTRAN #defines, adapt
   the generator script fortgen.pl instead */

#if defined (F_UPPER) && defined (F_SFX_USC)
#define pomp_init_F POMP_INIT_
#define pomp_on_F POMP_ON_
#define pomp_off_F POMP_OFF_
#define pomp_begin_F POMP_BEGIN_
#define pomp_end_F POMP_END_
#define pomp_atomic_enter_F POMP_ATOMIC_ENTER_
#define pomp_atomic_exit_F POMP_ATOMIC_EXIT_
#define pomp_barrier_enter_F POMP_BARRIER_ENTER_
#define pomp_barrier_exit_F POMP_BARRIER_EXIT_
#define pomp_critical_begin_F POMP_CRITICAL_BEGIN_
#define pomp_critical_end_F POMP_CRITICAL_END_
#define pomp_critical_enter_F POMP_CRITICAL_ENTER_
#define pomp_critical_exit_F POMP_CRITICAL_EXIT_
#define pomp_do_enter_F POMP_DO_ENTER_
#define pomp_do_exit_F POMP_DO_EXIT_
#define pomp_flush_enter_F POMP_FLUSH_ENTER_
#define pomp_flush_exit_F POMP_FLUSH_EXIT_
#define pomp_for_enter_F POMP_FOR_ENTER_
#define pomp_for_exit_F POMP_FOR_EXIT_
#define pomp_master_begin_F POMP_MASTER_BEGIN_
#define pomp_master_end_F POMP_MASTER_END_
#define pomp_parallel_begin_F POMP_PARALLEL_BEGIN_
#define pomp_parallel_end_F POMP_PARALLEL_END_
#define pomp_parallel_fork_F POMP_PARALLEL_FORK_
#define pomp_parallel_join_F POMP_PARALLEL_JOIN_
#define pomp_section_begin_F POMP_SECTION_BEGIN_
#define pomp_section_end_F POMP_SECTION_END_
#define pomp_sections_enter_F POMP_SECTIONS_ENTER_
#define pomp_sections_exit_F POMP_SECTIONS_EXIT_
#define pomp_single_begin_F POMP_SINGLE_BEGIN_
#define pomp_single_end_F POMP_SINGLE_END_
#define pomp_single_enter_F POMP_SINGLE_ENTER_
#define pomp_single_exit_F POMP_SINGLE_EXIT_
#define pomp_workshare_enter_F POMP_WORKSHARE_ENTER_
#define pomp_workshare_exit_F POMP_WORKSHARE_EXIT_
#define pomp_init_lock_F POMP_INIT_LOCK_
#define pomp_destroy_lock_F POMP_DESTROY_LOCK_
#define pomp_set_lock_F POMP_SET_LOCK_
#define pomp_unset_lock_F POMP_UNSET_LOCK_

#define ompp_papi_set_drange_F OMPP_PAPI_SET_DRANGE_

#endif

#if defined (F_UPPER) && defined (F_SFX_2USC)
#define pomp_init_F POMP_INIT__
#define pomp_on_F POMP_ON__
#define pomp_off_F POMP_OFF__
#define pomp_begin_F POMP_BEGIN__
#define pomp_end_F POMP_END__
#define pomp_atomic_enter_F POMP_ATOMIC_ENTER__
#define pomp_atomic_exit_F POMP_ATOMIC_EXIT__
#define pomp_barrier_enter_F POMP_BARRIER_ENTER__
#define pomp_barrier_exit_F POMP_BARRIER_EXIT__
#define pomp_critical_begin_F POMP_CRITICAL_BEGIN__
#define pomp_critical_end_F POMP_CRITICAL_END__
#define pomp_critical_enter_F POMP_CRITICAL_ENTER__
#define pomp_critical_exit_F POMP_CRITICAL_EXIT__
#define pomp_do_enter_F POMP_DO_ENTER__
#define pomp_do_exit_F POMP_DO_EXIT__
#define pomp_flush_enter_F POMP_FLUSH_ENTER__
#define pomp_flush_exit_F POMP_FLUSH_EXIT__
#define pomp_for_enter_F POMP_FOR_ENTER__
#define pomp_for_exit_F POMP_FOR_EXIT__
#define pomp_master_begin_F POMP_MASTER_BEGIN__
#define pomp_master_end_F POMP_MASTER_END__
#define pomp_parallel_begin_F POMP_PARALLEL_BEGIN__
#define pomp_parallel_end_F POMP_PARALLEL_END__
#define pomp_parallel_fork_F POMP_PARALLEL_FORK__
#define pomp_parallel_join_F POMP_PARALLEL_JOIN__
#define pomp_section_begin_F POMP_SECTION_BEGIN__
#define pomp_section_end_F POMP_SECTION_END__
#define pomp_sections_enter_F POMP_SECTIONS_ENTER__
#define pomp_sections_exit_F POMP_SECTIONS_EXIT__
#define pomp_single_begin_F POMP_SINGLE_BEGIN__
#define pomp_single_end_F POMP_SINGLE_END__
#define pomp_single_enter_F POMP_SINGLE_ENTER__
#define pomp_single_exit_F POMP_SINGLE_EXIT__
#define pomp_workshare_enter_F POMP_WORKSHARE_ENTER__
#define pomp_workshare_exit_F POMP_WORKSHARE_EXIT__
#define pomp_init_lock_F POMP_INIT_LOCK__
#define pomp_destroy_lock_F POMP_DESTROY_LOCK__
#define pomp_set_lock_F POMP_SET_LOCK__
#define pomp_unset_lock_F POMP_UNSET_LOCK__

#define ompp_papi_set_drange_F OMPP_PAPI_SET_DRANGE__

#endif

#if defined (F_LOWER) && defined (F_SFX_USC)
#define pomp_init_F pomp_init_
#define pomp_on_F pomp_on_
#define pomp_off_F pomp_off_
#define pomp_begin_F pomp_begin_
#define pomp_end_F pomp_end_
#define pomp_atomic_enter_F pomp_atomic_enter_
#define pomp_atomic_exit_F pomp_atomic_exit_
#define pomp_barrier_enter_F pomp_barrier_enter_
#define pomp_barrier_exit_F pomp_barrier_exit_
#define pomp_critical_begin_F pomp_critical_begin_
#define pomp_critical_end_F pomp_critical_end_
#define pomp_critical_enter_F pomp_critical_enter_
#define pomp_critical_exit_F pomp_critical_exit_
#define pomp_do_enter_F pomp_do_enter_
#define pomp_do_exit_F pomp_do_exit_
#define pomp_flush_enter_F pomp_flush_enter_
#define pomp_flush_exit_F pomp_flush_exit_
#define pomp_for_enter_F pomp_for_enter_
#define pomp_for_exit_F pomp_for_exit_
#define pomp_master_begin_F pomp_master_begin_
#define pomp_master_end_F pomp_master_end_
#define pomp_parallel_begin_F pomp_parallel_begin_
#define pomp_parallel_end_F pomp_parallel_end_
#define pomp_parallel_fork_F pomp_parallel_fork_
#define pomp_parallel_join_F pomp_parallel_join_
#define pomp_section_begin_F pomp_section_begin_
#define pomp_section_end_F pomp_section_end_
#define pomp_sections_enter_F pomp_sections_enter_
#define pomp_sections_exit_F pomp_sections_exit_
#define pomp_single_begin_F pomp_single_begin_
#define pomp_single_end_F pomp_single_end_
#define pomp_single_enter_F pomp_single_enter_
#define pomp_single_exit_F pomp_single_exit_
#define pomp_workshare_enter_F pomp_workshare_enter_
#define pomp_workshare_exit_F pomp_workshare_exit_
#define pomp_init_lock_F pomp_init_lock_
#define pomp_destroy_lock_F pomp_destroy_lock_
#define pomp_set_lock_F pomp_set_lock_
#define pomp_unset_lock_F pomp_unset_lock_

#define ompp_papi_set_drange_F ompp_papi_set_drange_
#endif

#if defined (F_LOWER) && defined (F_SFX_2USC)
#define pomp_init_F pomp_init__
#define pomp_on_F pomp_on__
#define pomp_off_F pomp_off__
#define pomp_begin_F pomp_begin__
#define pomp_end_F pomp_end__
#define pomp_atomic_enter_F pomp_atomic_enter__
#define pomp_atomic_exit_F pomp_atomic_exit__
#define pomp_barrier_enter_F pomp_barrier_enter__
#define pomp_barrier_exit_F pomp_barrier_exit__
#define pomp_critical_begin_F pomp_critical_begin__
#define pomp_critical_end_F pomp_critical_end__
#define pomp_critical_enter_F pomp_critical_enter__
#define pomp_critical_exit_F pomp_critical_exit__
#define pomp_do_enter_F pomp_do_enter__
#define pomp_do_exit_F pomp_do_exit__
#define pomp_flush_enter_F pomp_flush_enter__
#define pomp_flush_exit_F pomp_flush_exit__
#define pomp_for_enter_F pomp_for_enter__
#define pomp_for_exit_F pomp_for_exit__
#define pomp_master_begin_F pomp_master_begin__
#define pomp_master_end_F pomp_master_end__
#define pomp_parallel_begin_F pomp_parallel_begin__
#define pomp_parallel_end_F pomp_parallel_end__
#define pomp_parallel_fork_F pomp_parallel_fork__
#define pomp_parallel_join_F pomp_parallel_join__
#define pomp_section_begin_F pomp_section_begin__
#define pomp_section_end_F pomp_section_end__
#define pomp_sections_enter_F pomp_sections_enter__
#define pomp_sections_exit_F pomp_sections_exit__
#define pomp_single_begin_F pomp_single_begin__
#define pomp_single_end_F pomp_single_end__
#define pomp_single_enter_F pomp_single_enter__
#define pomp_single_exit_F pomp_single_exit__
#define pomp_workshare_enter_F pomp_workshare_enter__
#define pomp_workshare_exit_F pomp_workshare_exit__
#define pomp_init_lock_F pomp_init_lock__
#define pomp_destroy_lock_F pomp_destroy_lock__
#define pomp_set_lock_F pomp_set_lock__
#define pomp_unset_lock_F pomp_unset_lock__

#define ompp_papi_set_drange_F ompp_papi_set_drange___
#endif


#if defined (F_UPPER) && defined (F_NO_SFX)
#define pomp_init_F POMP_INIT
#define pomp_on_F POMP_ON
#define pomp_off_F POMP_OFF
#define pomp_begin_F POMP_BEGIN
#define pomp_end_F POMP_END
#define pomp_atomic_enter_F POMP_ATOMIC_ENTER
#define pomp_atomic_exit_F POMP_ATOMIC_EXIT
#define pomp_barrier_enter_F POMP_BARRIER_ENTER
#define pomp_barrier_exit_F POMP_BARRIER_EXIT
#define pomp_critical_begin_F POMP_CRITICAL_BEGIN
#define pomp_critical_end_F POMP_CRITICAL_END
#define pomp_critical_enter_F POMP_CRITICAL_ENTER
#define pomp_critical_exit_F POMP_CRITICAL_EXIT
#define pomp_do_enter_F POMP_DO_ENTER
#define pomp_do_exit_F POMP_DO_EXIT
#define pomp_flush_enter_F POMP_FLUSH_ENTER
#define pomp_flush_exit_F POMP_FLUSH_EXIT
#define pomp_for_enter_F POMP_FOR_ENTER
#define pomp_for_exit_F POMP_FOR_EXIT
#define pomp_master_begin_F POMP_MASTER_BEGIN
#define pomp_master_end_F POMP_MASTER_END
#define pomp_parallel_begin_F POMP_PARALLEL_BEGIN
#define pomp_parallel_end_F POMP_PARALLEL_END
#define pomp_parallel_fork_F POMP_PARALLEL_FORK
#define pomp_parallel_join_F POMP_PARALLEL_JOIN
#define pomp_section_begin_F POMP_SECTION_BEGIN
#define pomp_section_end_F POMP_SECTION_END
#define pomp_sections_enter_F POMP_SECTIONS_ENTER
#define pomp_sections_exit_F POMP_SECTIONS_EXIT
#define pomp_single_begin_F POMP_SINGLE_BEGIN
#define pomp_single_end_F POMP_SINGLE_END
#define pomp_single_enter_F POMP_SINGLE_ENTER
#define pomp_single_exit_F POMP_SINGLE_EXIT
#define pomp_workshare_enter_F POMP_WORKSHARE_ENTER
#define pomp_workshare_exit_F POMP_WORKSHARE_EXIT
#define pomp_init_lock_F POMP_INIT_LOCK
#define pomp_destroy_lock_F POMP_DESTROY_LOCK
#define pomp_set_lock_F POMP_SET_LOCK
#define pomp_unset_lock_F POMP_UNSET_LOCK

#define ompp_papi_set_drange_F OMPP_PAPI_SET_DRANGE

#endif

#if defined (F_LOWER) && defined (F_NO_SFX)
#define pomp_init_F pomp_init
#define pomp_on_F pomp_on
#define pomp_off_F pomp_off
#define pomp_begin_F pomp_begin
#define pomp_end_F pomp_end
#define pomp_atomic_enter_F pomp_atomic_enter
#define pomp_atomic_exit_F pomp_atomic_exit
#define pomp_barrier_enter_F pomp_barrier_enter
#define pomp_barrier_exit_F pomp_barrier_exit
#define pomp_critical_begin_F pomp_critical_begin
#define pomp_critical_end_F pomp_critical_end
#define pomp_critical_enter_F pomp_critical_enter
#define pomp_critical_exit_F pomp_critical_exit
#define pomp_do_enter_F pomp_do_enter
#define pomp_do_exit_F pomp_do_exit
#define pomp_flush_enter_F pomp_flush_enter
#define pomp_flush_exit_F pomp_flush_exit
#define pomp_for_enter_F pomp_for_enter
#define pomp_for_exit_F pomp_for_exit
#define pomp_master_begin_F pomp_master_begin
#define pomp_master_end_F pomp_master_end
#define pomp_parallel_begin_F pomp_parallel_begin
#define pomp_parallel_end_F pomp_parallel_end
#define pomp_parallel_fork_F pomp_parallel_fork
#define pomp_parallel_join_F pomp_parallel_join
#define pomp_section_begin_F pomp_section_begin
#define pomp_section_end_F pomp_section_end
#define pomp_sections_enter_F pomp_sections_enter
#define pomp_sections_exit_F pomp_sections_exit
#define pomp_single_begin_F pomp_single_begin
#define pomp_single_end_F pomp_single_end
#define pomp_single_enter_F pomp_single_enter
#define pomp_single_exit_F pomp_single_exit
#define pomp_workshare_enter_F pomp_workshare_enter
#define pomp_workshare_exit_F pomp_workshare_exit
#define pomp_init_lock_F pomp_init_lock
#define pomp_destroy_lock_F pomp_destroy_lock
#define pomp_set_lock_F pomp_set_lock
#define pomp_unset_lock_F pomp_unset_lock

#define ompp_papi_set_drange_F ompp_papi_set_drange

#endif

void pomp_init_F()
{
  POMP_Init();
}


void pomp_on_F()
{
  POMP_On();
}

void pomp_off_F()
{
  POMP_Off();
}

void pomp_begin_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Begin(r);
}

void pomp_end_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_End(r);
}

void pomp_atomic_enter_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Atomic_enter(r);
}

void pomp_atomic_exit_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Atomic_exit(r);
}

void pomp_barrier_enter_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Barrier_enter(r);
}

void pomp_barrier_exit_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Barrier_exit(r);
}

void pomp_critical_begin_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Critical_begin(r);
}

void pomp_critical_end_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Critical_end(r);
}

void pomp_critical_enter_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Critical_enter(r);
}

void pomp_critical_exit_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Critical_exit(r);
}

void pomp_do_enter_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r )  POMP_Do_enter(r);
}

void pomp_do_exit_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Do_exit(r);
}

void pomp_flush_enter_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Flush_enter(r);
}

void pomp_flush_exit_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Flush_exit(r);
}

void pomp_for_enter_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_For_enter(r);
}

void pomp_for_exit_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_For_exit(r);
}

void pomp_master_begin_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Master_begin(r);
}

void pomp_master_end_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Master_end(r);
}

void pomp_parallel_begin_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Parallel_begin(r);
}

void pomp_parallel_end_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Parallel_end(r);
}

void pomp_parallel_fork_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Parallel_fork(r);
}

void pomp_parallel_join_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Parallel_join(r);
}


void pomp_section_begin_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Section_begin(r);
}

void pomp_section_end_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Section_end(r);
}

void pomp_sections_enter_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Sections_enter(r);
}

void pomp_sections_exit_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Sections_exit(r);
}

void pomp_single_begin_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Single_begin(r);
}

void pomp_single_end_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Single_end(r);
}

void pomp_single_enter_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Single_enter(r);
}

void pomp_single_exit_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Single_exit(r);
}

void pomp_workshare_enter_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Workshare_enter(r);
}

void pomp_workshare_exit_F( void *n )
{
  struct ompregdescr *r = pomp_rd_table[*((int*)n)];
  if( r ) POMP_Workshare_exit(r);
}

//
// LOCKS
// 

void pomp_init_lock_F(void *n)
{
  POMP_Init_lock((omp_lock_t*)n);
}

void pomp_destroy_lock_F(void *n)
{
  POMP_Destroy_lock((omp_lock_t*)n);
}

void pomp_set_lock_F(void *n )
{
  POMP_Set_lock((omp_lock_t*)n);
}

void pomp_unset_lock_F(void *n )
{
  POMP_Unset_lock((omp_lock_t*)n);
}


#ifdef OMPP_DATARANGE

void ompp_papi_set_drange_F( void* addr, 
			     unsigned int *size, char* name )
{
  OMPP_papi_set_drange( addr, (unsigned int)(*size), name );
}

#endif /* OMPP_DATARANGE */



