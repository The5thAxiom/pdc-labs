/* 
 * ompp_pomp.h  */
/* @OMPP_HEADER@ */

#ifndef OMPP_POMP_H_INCLUDED
#define OMPP_POMP_H_INCLUDED


#include "pomp_lib.h"
#include "ompp_region.h"

extern omp_lock_t dump_lock;
extern omp_lock_t make_region_lock;

//
// all POMP enter/exit fork/join functions
// are mapped to only this one pair of enter/exit calls
// and the particular ompp_reg_type_t determines if its a fork or join,
// enter or exit call, etc. 
//

void ompp_pomp_region_enter( struct ompregdescr *r, 
			     ompp_reg_type_t );


void ompp_pomp_region_exit( struct ompregdescr *r, 
			    ompp_reg_type_t );


//
// LOCK-related functions
//
void ompp_pomp_init_lock(omp_lock_t *lock);
void ompp_pomp_destroy_lock(omp_lock_t *lock);
void ompp_pomp_set_lock(omp_lock_t *lock);
void ompp_pomp_unset_lock(omp_lock_t *lock);
int  ompp_pomp_test_lock(omp_lock_t *lock);


void ompp_pomp_initialize(void);


ompp_region_t* ompp_get_region_for_descr( struct ompregdescr *r, 
					  ompp_reg_type_t type );

ompp_region_t* ompp_get_region_for_lock( omp_lock_t *lock );


#endif /* OMPP_POMP_H_INCLUDED */
