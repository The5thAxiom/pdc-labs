/*
 * ompp_properties.h  */
/* @OMPP_HEADER@ */

#ifndef OMPP_PROPERTIES_H_INCLUDED
#define OMPP_PROPERTIES_H_INCLUDED


typedef enum
  {
    OMPP_PERFPROP_WaitAtBarrier = 0,
    OMPP_PERFPROP_ImbalanceInParallelRegion,
    OMPP_PERFPROP_ImbalanceInParallelLoop,
    OMPP_PERFPROP_ImbalanceInParallelWorkshare,
    OMPP_PERFPROP_ImbalanceInParallelSections,
    OMPP_PERFPROP_ImbalanceDueToNotEnoughSections,
    OMPP_PERFPROP_ImbalanceDueToUnevenSectionDistribution,
    OMPP_PERFPROP_CriticalSectionContention,
    OMPP_PERFPROP_LockContention,
    OMPP_PERFPROP_FrequentAtomic,
    OMPP_PERFPROP_InsufficienWorkInParallelLoop,
    OMPP_PERFPROP_UnparallelizedInMasterRegion,
    OMPP_PERFPROP_UnparallelizedInSingleRegion,
    OMPP_PERFPROP_LAST
  }
ompp_perfprop_type_t;

//
// holds the name for each property type
//
extern const char* ompp_perfprop_name[];


typedef struct
{
  ompp_perfprop_type_t type;
  double               severity;
  char                 location[80];
}
ompp_perfprop_t;



// 
// function prototypes
//
void ompp_perfprop_initialize(void);
void ompp_perfprop_print(FILE *f, ompp_perfprop_t *prop);
unsigned ompp_perfprop_check(dllist_t *reglist, ompp_perfprop_t *props, unsigned nreg );
int perfprop_compare_by_severtiy( const void *p1, const void *p2 );



#endif /* OMPP_PROPERTIES_H_INCLUDED */
