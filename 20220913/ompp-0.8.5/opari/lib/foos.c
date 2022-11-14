/****************************************************************************
**  SCALASCA    http://www.scalasca.org/                                   **
**  KOJAK       http://www.fz-juelich.de/jsc/kojak/                        **
*****************************************************************************
**  Copyright (c) 1998-2009                                                **
**  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
**                                                                         **
**  See the file COPYRIGHT in the package base directory for details       **
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

static char *header =

"/*\n"
" * Fortan subroutine external name setup\n"
" */\n"
"\n"
"#define POMP_Finalize_U		POMP_FINALIZE\n"
"#define POMP_Init_U		POMP_INIT\n"
"#define POMP_Off_U		POMP_OFF\n"
"#define POMP_On_U		POMP_ON\n"
"#define POMP_Atomic_enter_U	POMP_ATOMIC_ENTER\n"
"#define POMP_Atomic_exit_U	POMP_ATOMIC_EXIT\n"
"#define POMP_Barrier_enter_U	POMP_BARRIER_ENTER\n"
"#define POMP_Barrier_exit_U	POMP_BARRIER_EXIT\n"
"#define POMP_Flush_enter_U	POMP_FLUSH_ENTER\n"
"#define POMP_Flush_exit_U	POMP_FLUSH_EXIT\n"
"#define POMP_Critical_begin_U	POMP_CRITICAL_BEGIN\n"
"#define POMP_Critical_end_U	POMP_CRITICAL_END\n"
"#define POMP_Critical_enter_U	POMP_CRITICAL_ENTER\n"
"#define POMP_Critical_exit_U	POMP_CRITICAL_EXIT\n"
"#define POMP_Do_enter_U		POMP_DO_ENTER\n"
"#define POMP_Do_exit_U		POMP_DO_EXIT\n"
"#define POMP_Master_begin_U	POMP_MASTER_BEGIN\n"
"#define POMP_Master_end_U	POMP_MASTER_END\n"
"#define POMP_Parallel_begin_U	POMP_PARALLEL_BEGIN\n"
"#define POMP_Parallel_end_U	POMP_PARALLEL_END\n"
"#define POMP_Parallel_fork_U	POMP_PARALLEL_FORK\n"
"#define POMP_Parallel_join_U	POMP_PARALLEL_JOIN\n"
"#define POMP_Section_begin_U	POMP_SECTION_BEGIN\n"
"#define POMP_Section_end_U	POMP_SECTION_END\n"
"#define POMP_Sections_enter_U	POMP_SECTIONS_ENTER\n"
"#define POMP_Sections_exit_U	POMP_SECTIONS_EXIT\n"
"#define POMP_Single_begin_U	POMP_SINGLE_BEGIN\n"
"#define POMP_Single_end_U	POMP_SINGLE_END\n"
"#define POMP_Single_enter_U	POMP_SINGLE_ENTER\n"
"#define POMP_Single_exit_U	POMP_SINGLE_EXIT\n"
"#define POMP_Workshare_enter_U	POMP_WORKSHARE_ENTER\n"
"#define POMP_Workshare_exit_U	POMP_WORKSHARE_EXIT\n"
"#define POMP_Begin_U		POMP_BEGIN\n"
"#define POMP_End_U		POMP_END\n"
"#define POMP_Init_lock_U	POMP_INIT_LOCK\n"
"#define POMP_Destroy_lock_U	POMP_DESTROY_LOCK\n"
"#define POMP_Set_lock_U		POMP_SET_LOCK\n"
"#define POMP_Unset_lock_U	POMP_UNSET_LOCK\n"
"#define POMP_Test_lock_U	POMP_TEST_LOCK\n"
"#define POMP_Init_nest_lock_U	POMP_INIT_NEST_LOCK\n"
"#define POMP_Destroy_nest_lock_U	POMP_DESTROY_NEST_LOCK\n"
"#define POMP_Set_nest_lock_U	POMP_SET_NEST_LOCK\n"
"#define POMP_Unset_nest_lock_U	POMP_UNSET_NEST_LOCK\n"
"#define POMP_Test_nest_lock_U	POMP_TEST_NEST_LOCK\n"
"#define omp_init_lock_U		OMP_INIT_LOCK\n"
"#define omp_destroy_lock_U	OMP_DESTROY_LOCK\n"
"#define omp_set_lock_U		OMP_SET_LOCK\n"
"#define omp_unset_lock_U	OMP_UNSET_LOCK\n"
"#define omp_test_lock_U		OMP_TEST_LOCK\n"
"#define omp_init_nest_lock_U	OMP_INIT_NEST_LOCK\n"
"#define omp_destroy_nest_lock_U	OMP_DESTROY_NEST_LOCK\n"
"#define omp_set_nest_lock_U	OMP_SET_NEST_LOCK\n"
"#define omp_unset_nest_lock_U	OMP_UNSET_NEST_LOCK\n"
"#define omp_test_nest_lock_U	OMP_TEST_NEST_LOCK\n"
"\n"
"#define POMP_Finalize_L		pomp_finalize\n"
"#define POMP_Init_L		pomp_init\n"
"#define POMP_Off_L		pomp_off\n"
"#define POMP_On_L		pomp_on\n"
"#define POMP_Atomic_enter_L	pomp_atomic_enter\n"
"#define POMP_Atomic_exit_L	pomp_atomic_exit\n"
"#define POMP_Barrier_enter_L	pomp_barrier_enter\n"
"#define POMP_Barrier_exit_L	pomp_barrier_exit\n"
"#define POMP_Flush_enter_L	pomp_flush_enter\n"
"#define POMP_Flush_exit_L	pomp_flush_exit\n"
"#define POMP_Critical_begin_L	pomp_critical_begin\n"
"#define POMP_Critical_end_L	pomp_critical_end\n"
"#define POMP_Critical_enter_L	pomp_critical_enter\n"
"#define POMP_Critical_exit_L	pomp_critical_exit\n"
"#define POMP_Do_enter_L		pomp_do_enter\n"
"#define POMP_Do_exit_L		pomp_do_exit\n"
"#define POMP_Master_begin_L	pomp_master_begin\n"
"#define POMP_Master_end_L	pomp_master_end\n"
"#define POMP_Parallel_begin_L	pomp_parallel_begin\n"
"#define POMP_Parallel_end_L	pomp_parallel_end\n"
"#define POMP_Parallel_fork_L	pomp_parallel_fork\n"
"#define POMP_Parallel_join_L	pomp_parallel_join\n"
"#define POMP_Section_begin_L	pomp_section_begin\n"
"#define POMP_Section_end_L	pomp_section_end\n"
"#define POMP_Sections_enter_L	pomp_sections_enter\n"
"#define POMP_Sections_exit_L	pomp_sections_exit\n"
"#define POMP_Single_begin_L	pomp_single_begin\n"
"#define POMP_Single_end_L	pomp_single_end\n"
"#define POMP_Single_enter_L	pomp_single_enter\n"
"#define POMP_Single_exit_L	pomp_single_exit\n"
"#define POMP_Workshare_enter_L	pomp_workshare_enter\n"
"#define POMP_Workshare_exit_L	pomp_workshare_exit\n"
"#define POMP_Begin_L		pomp_begin\n"
"#define POMP_End_L		pomp_end\n"
"#define POMP_Init_lock_L	pomp_init_lock\n"
"#define POMP_Destroy_lock_L	pomp_destroy_lock\n"
"#define POMP_Set_lock_L		pomp_set_lock\n"
"#define POMP_Unset_lock_L	pomp_unset_lock\n"
"#define POMP_Test_lock_L	pomp_test_lock\n"
"#define POMP_Init_nest_lock_L	pomp_init_nest_lock\n"
"#define POMP_Destroy_nest_lock_L	pomp_destroy_nest_lock\n"
"#define POMP_Set_nest_lock_L	pomp_set_nest_lock\n"
"#define POMP_Unset_nest_lock_L	pomp_unset_nest_lock\n"
"#define POMP_Test_nest_lock_L	pomp_test_nest_lock\n"
"#define omp_init_lock_L		omp_init_lock\n"
"#define omp_destroy_lock_L	omp_destroy_lock\n"
"#define omp_set_lock_L		omp_set_lock\n"
"#define omp_unset_lock_L	omp_unset_lock\n"
"#define omp_test_lock_L		omp_test_lock\n"
"#define omp_init_nest_lock_L	omp_init_nest_lock\n"
"#define omp_destroy_nest_lock_L	omp_destroy_nest_lock\n"
"#define omp_set_nest_lock_L	omp_set_nest_lock\n"
"#define omp_unset_nest_lock_L	omp_unset_nest_lock\n"
"#define omp_test_nest_lock_L	omp_test_nest_lock\n"
"\n"
"#define XSUFFIX(name)  name##_\n"
"#define XSUFFIX2(name) name##__\n"
"#define XPREFIX(name)  _##name\n"
"#define XPREFIX2(name) __##name\n"
"\n"
"#define SUFFIX(name)  XSUFFIX(name)\n"
"#define SUFFIX2(name) XSUFFIX2(name)\n"
"#define PREFIX(name)  XPREFIX(name)\n"
"#define PREFIX2(name) XPREFIX2(name)\n"
"\n"
"#define UPCASE(name)  name##_U\n"
"#define LOWCASE(name) name##_L\n"
"\n";

static void generate(char *str) {
  FILE* f = fopen("pomp_fwrapper_def.h", "w");
  if (f == NULL) { perror("pomp_fwrapper_def.h"); exit(1); }
  fputs(header, f);
  fputs(str, f);
  fputs("\n", f);
  fclose(f);
}

void   foo_foo()   { generate("#define FSUB(name) LOWCASE(name)"); }
void   foo_foo_()  { generate("#define FSUB(name) SUFFIX(LOWCASE(name))"); }
void   foo_foo__() { generate("#define FSUB(name) SUFFIX2(LOWCASE(name))"); }
void  _foo_foo()   { generate("#define FSUB(name) PREFIX(LOWCASE(name))"); }
void __foo_foo()   { generate("#define FSUB(name) PREFIX2(LOWCASE(name))"); }
void  _foo_foo_()  { generate("#define FSUB(name) PREFIX(SUFFIX(LOWCASE(name)))"); }
void   FOO_FOO()   { generate("#define FSUB(name) UPCASE(name)"); }
void   FOO_FOO_()  { generate("#define FSUB(name) SUFFIX(UPCASE(name))"); }
void   FOO_FOO__() { generate("#define FSUB(name) SUFFIX2(UPCASE(name))"); }
void  _FOO_FOO()   { generate("#define FSUB(name) PREFIX(UPCASE(name))"); }
void __FOO_FOO()   { generate("#define FSUB(name) PREFIX2(UPCASE(name))"); }
void  _FOO_FOO_()  { generate("#define FSUB(name) PREFIX(SUFFIX(UPCASE(name)))"); }
