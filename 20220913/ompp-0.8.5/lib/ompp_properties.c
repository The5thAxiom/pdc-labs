/*
 * ompp_properties.c  */
/* @OMPP_HEADER@ */

#include <assert.h>

#include "utils/list.h"
#include "ompp_regstat.h"
#include "ompp_region.h"
#include "ompp_properties.h"


const char *ompp_perfprop_name[OMPP_PERFPROP_LAST];


//
// initialize the property names...
//
void ompp_perfprop_initialize()
{
  ompp_perfprop_name[OMPP_PERFPROP_WaitAtBarrier]
    ="WaitAtBarrier";

  ompp_perfprop_name[OMPP_PERFPROP_ImbalanceInParallelRegion]
    ="ImbalanceInParallelRegion";

  ompp_perfprop_name[OMPP_PERFPROP_ImbalanceInParallelLoop]
    ="ImbalanceInParallelLoop";

  ompp_perfprop_name[OMPP_PERFPROP_ImbalanceInParallelWorkshare]
    ="ImbalanceInParallelWorkshare";

  ompp_perfprop_name[OMPP_PERFPROP_ImbalanceInParallelSections]
    ="ImbalanceInParallelSections";

  ompp_perfprop_name[OMPP_PERFPROP_ImbalanceDueToNotEnoughSections]
    ="ImbalanceDueToNotEnoughSections";

  ompp_perfprop_name[OMPP_PERFPROP_ImbalanceDueToUnevenSectionDistribution]
    ="ImbalanceDueToUnevenSectionDistribution";

  ompp_perfprop_name[OMPP_PERFPROP_CriticalSectionContention]
    ="CriticalSectionContention";

  ompp_perfprop_name[OMPP_PERFPROP_LockContention]
    ="LockContention";
  
  ompp_perfprop_name[OMPP_PERFPROP_FrequentAtomic]
    ="FrequentAtomic";

  ompp_perfprop_name[OMPP_PERFPROP_InsufficienWorkInParallelLoop]
    ="InsufficienWorkInParallelLoop";

  ompp_perfprop_name[OMPP_PERFPROP_UnparallelizedInMasterRegion]
    ="UnparallelizedInMasterRegion";

  ompp_perfprop_name[OMPP_PERFPROP_UnparallelizedInSingleRegion]
    ="UnparallelizedInSingleRegion";
}


/*
 * take a regstat structure and try to instantiate
 * properties...
 */
ompp_perfprop_t* instantiate_property( regstat_t *stat )
{
  unsigned sumSect, sumExec, minC, maxC;
  ompp_perfprop_t *prop=0;
  double d;

  // ranking basis
  double rb = ompp_duration*ompp_max_threads;

  assert( stat );
  assert( stat->reg );

  prop = (ompp_perfprop_t*)malloc( sizeof(ompp_perfprop_t) );
  assert(prop);
  
  switch( stat->reg->type )
    {
    case OMPP_BARRIER:
      
      // *************
      // WaitAtBarrier
      // *************
      assert( stat->main );
      //OLD d = mstat_sumt( stat->main );
      MSTAT_SUMT( stat->main, d );

      prop->type      = OMPP_PERFPROP_WaitAtBarrier;
      prop->severity  = d/rb;
      break;

    case OMPP_PARALLEL:

      // *************************
      // ImbalanceInParallelRegion
      // *************************
      assert( stat->ibarr );
      //OLD d = mstat_sumt( stat->ibarr );
      MSTAT_SUMT( stat->ibarr, d );

      prop->type      = OMPP_PERFPROP_ImbalanceInParallelRegion;
      prop->severity  = d/rb;
      break;

    case OMPP_LOOP:
    case OMPP_PARALLEL_LOOP:
      
      // ***********************
      // ImbalanceInParallelLoop
      // ***********************
      assert( stat->ibarr );
      //OLD d = mstat_sumt( stat->ibarr );
      MSTAT_SUMT( stat->ibarr, d );

      prop->type      = OMPP_PERFPROP_ImbalanceInParallelLoop;
      prop->severity  = d/rb;
      break;

    case OMPP_WORKSHARE:
    case OMPP_PARALLEL_WORKSHARE:
      
      // ****************************
      // ImbalanceInParallelWorkshare
      // ****************************
      assert( stat->ibarr );
      //OLD d = mstat_sumt( stat->ibarr );
      MSTAT_SUMT( stat->ibarr, d );

      prop->type      = OMPP_PERFPROP_ImbalanceInParallelWorkshare;
      prop->severity  = d/rb;
      break;


    case OMPP_SECTIONS:
    case OMPP_PARALLEL_SECTIONS:
     
      assert( stat->ibarr );
      //OLD d = mstat_sumt( stat->ibarr );
      MSTAT_SUMT( stat->ibarr, d );
      
      // total number of sections executed by all threads
      //OLD sumSect = mstat_sumc( stat->body );
      MSTAT_SUMC( stat->body, sumSect );
      
      // total number of invocations of all threads
      //OLD sumExec = mstat_sumc( stat->main );
      MSTAT_SUMC( stat->main, sumExec );

      // minimum and maximum number of sections executed
      // by the threads
      //OLD minC = mstat_minc( stat->body );
      //OLD maxC = mstat_maxc( stat->body );
      MSTAT_MINC( stat->body, minC );
      MSTAT_MAXC( stat->body, maxC );


      if( ((maxC-minC)>0) && (sumSect>sumExec) ) 
	{
	  // ***************************************
	  // ImbalanceDueToUnevenSectionDistribution
	  // ***************************************
	  
	  prop->type      = OMPP_PERFPROP_ImbalanceDueToUnevenSectionDistribution;
	  prop->severity  = d/rb;
	  break;
	}

      if( ((maxC-minC)>0) && (sumSect<sumExec) ) 
	{
	  // *******************************
	  // ImbalanceDueToNotEnoughSections
	  // *******************************
	  
	  prop->type      = OMPP_PERFPROP_ImbalanceDueToNotEnoughSections;
	  prop->severity  = d/rb;
	  break;
	}

      // ***************************
      // ImbalanceInParallelSections
      // ***************************
      prop->type      = OMPP_PERFPROP_ImbalanceInParallelSections;
      prop->severity  = d/rb;
      break;

    case OMPP_CRITICAL:

      // *************************
      // CriticalSectionContention
      // *************************
      
      assert( stat->enter );
      //OLD d = mstat_sumt( stat->enter );
      MSTAT_SUMT( stat->enter, d );

      prop->type      = OMPP_PERFPROP_CriticalSectionContention;
      prop->severity  = d/rb;

      break;

    case OMPP_LOCK:

      // *********************
      // LockSectionContention
      // *********************
      
      assert( stat->enter );
      //OLD d = mstat_sumt( stat->enter );
      MSTAT_SUMT( stat->enter, d );

      prop->type      = OMPP_PERFPROP_LockContention;
      prop->severity  = d/rb;
      
      break;

    case OMPP_ATOMIC:

      // **************
      // FrequentAtomic
      // **************

      assert( stat->main );
      //OLD d = mstat_sumt( stat->main );
      MSTAT_SUMT( stat->main, d );

      //old sumExec = mstat_sumc( stat->main );
      MSTAT_SUMC( stat->main, sumExec );

      // XXX: make this a configurable constant
      if( ((double)sumExec)/rb > 1000 )
	{
	  prop->type      = OMPP_PERFPROP_FrequentAtomic;
	  prop->severity  = d/rb;
	}
      else
	{
	  free(prop);
	  prop=0;
	}
      break;
	  

    case OMPP_SINGLE:

      // ****************************
      // UnparallelizedInSingleRegion
      // ****************************

      assert( stat->ibarr );
      //OLD d = mstat_sumt( stat->ibarr );
      MSTAT_SUMT( stat->ibarr, d );

      prop->type      = OMPP_PERFPROP_UnparallelizedInSingleRegion;
      prop->severity  = d/rb;

      break;

    case OMPP_MASTER:

      // ****************************
      // UnparallelizedInMasterRegion
      // ****************************

      assert( stat->main );
      //OLD d = mstat_sumt( stat->main );
      MSTAT_SUMT( stat->main, d );

      prop->type      = OMPP_PERFPROP_UnparallelizedInMasterRegion;
      prop->severity  = d/rb;

      break;

    default:
      free(prop);
      prop=0;
    }

  if( prop )
    {
      sprintf(prop->location, "%s %s", 
	      ompp_region_full_name[stat->reg->type], stat->reg->location);
    }

  return prop;
}


unsigned ompp_perfprop_check( dllist_t *reglist, 
			  ompp_perfprop_t *props, unsigned nreg )
{
  lelem_t *elem;
  ompp_region_t *reg;
  regstat_t *stat;
  unsigned i;
  ompp_perfprop_t *prop;
  

  //
  // go through the region list and try to 
  // intantiate properties
  //
  elem=reglist->head; 
  i=0;
  while( elem )
    {
      reg = (ompp_region_t*)elem->data;
      assert(reg);

      stat = reg->rstat;
      assert(stat);
      
      prop = instantiate_property(stat);
      if( prop )
	props[i++]=*prop;

      elem=elem->next;
    }
  
  // return the number of detected properties
  return i;
}


int perfprop_compare_by_severtiy( const void *p1, const void *p2 )
{
  ompp_perfprop_t *prop1 = (ompp_perfprop_t*)p1;
  ompp_perfprop_t *prop2 = (ompp_perfprop_t*)p2;

  return ((prop1->severity) < (prop2->severity));
}
