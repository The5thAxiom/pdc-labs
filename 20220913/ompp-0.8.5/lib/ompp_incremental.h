/*
 * ompp_incremental.h  */
/* @OMPP_HEADER@ */

#ifndef OMPP_INCREMENTAL_H_INCLUDED
#define OMPP_INCREMENTAL_H_INCLUDED

/*
 * checks if user wants profiling over time and
 * sets timers etc
 */
void ompp_profile_over_time_initialize(void);

void OMPP_Dump();

/* -- internals -- */
void ompp_handle_timer(int);



#endif /* OMPP_INCREMENTAL_H_INCLUDED */


