/* 
 * ompp_region.h  */
/* @OMPP_HEADER@ */

#ifndef OMPP_REGION_H_INCLUDED
#define OMPP_REGION_H_INCLUDED

#include "ompp.h"
#include "ompp_regstat.h"


typedef struct ompp_region
{
  struct ompregdescr *desc;
  ompp_reg_type_t     type;
  unsigned int        number;
  char                location[256];
  char                csvloc[256];
  regstat_t          *rstat;

  /* number of callgraph region stats for this
     region, does not including the flat stat
     the rstat pointer points to */
  int                 nstats;

  /*
  // the base parallel region
  struct ompp_region *par_base; 
  */

  struct ompp_region  *sub;
}
ompp_region_t;



#define OMPP_REGTYPE(region_) (ompp_region_name[region_->type])


void ompp_region_init( ompp_region_t *reg );

int ompp_region_get_static_number( ompp_region_t *reg );


ompp_region_t* ompp_get_region_by_number( unsigned num );


void ompp_region_enter( ompp_region_t *reg,
			ompp_reg_type_t type,
			int tid );

void ompp_region_exit( ompp_region_t *reg,
		       ompp_reg_type_t type,
		       int tid );


#endif /* OMPP_REGION_H_INCLUDED */
