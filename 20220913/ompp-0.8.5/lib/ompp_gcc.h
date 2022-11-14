/*
 * ompp_gcc.h  */
/* @OMPP_HEADER@ */

#ifdef __GNUC__

#include "ompp_region.h"

void ompp_gcc_initialize();


void __cyg_profile_func_enter(void *, void *) 
     __attribute__ ((no_instrument_function));

void __cyg_profile_func_exit(void *, void *)
     __attribute__ ((no_instrument_function));


ompp_region_t* ompp_get_region_for_gccfunc( void* func );

#endif 
