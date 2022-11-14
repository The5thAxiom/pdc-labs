/*
 * ompp_regstat.h  */
/* @OMPP_HEADER@ */

#ifndef OMPP_REGSTAT_H_INCLUDED
#define OMPP_REGSTAT_H_INCLUDED

#include <stdio.h>
#include "ompp.h"
#include "ompp_mpi.h"

struct regstat;
struct ompp_region;

/*
#define MSTAT_EXECC      1 
#define MSTAT_EXECT  (1<<1)
#define MSTAT_CTR    (1<<2)
*/


//
// micro-region statistics
//
struct mstat
{
  struct regstat *rstat;
  
  unsigned nexect;
  unsigned nexecc;
  unsigned nctr;

  unsigned *rlevel;


  // enter-timestamps to compute durations
  double   *exect_enter[OMPP_MAX_RECURSION];
  
  // sums execution time
  double   *exect;

  // sums exection counts
  unsigned *execc;

#ifdef OMPP_MPI
  double   *time;
  unsigned *nsend;
  unsigned *nrecv;
  unsigned *ncoll;
  unsigned *bout;
  unsigned *bin;

  double   *time_enter;
  unsigned *nsend_enter;
  unsigned *nrecv_enter;
  unsigned *ncoll_enter;
  unsigned *bout_enter;
  unsigned *bin_enter;
#endif /* OMPP_MPI */

#ifdef OMPP_USE_PAPI
  long long *ctr[OMPP_PAPI_MAX_CTRS];
  long long *ctr_enter[OMPP_PAPI_MAX_CTRS][OMPP_MAX_RECURSION];
#endif 
};

typedef struct mstat mstat_t;

typedef struct perfdata
{
  double    time;

#ifdef OMPP_MPI
  ompp_mpi_t mpi;
#endif /* OMPP_MPI */

#ifdef OMPP_USE_PAPI
  long long ctr[OMPP_PAPI_MAX_CTRS];
#endif /* OMPP_USE_PAPI */
}
perfdata_t;



#define MSTAT_SUMT_ADDTO( mstatptr_, sum_ )   \
{                                             \
  int i_;                                     \
                                              \
  for( i_=0; i_<(mstatptr_)->nexect; i_++ )   \
    {                                         \
      (sum_)+=((mstatptr_)->exect[i_]);       \
    }                                         \
}


#define MSTAT_SUMT_ADDTO_MPI( mstatptr_, sum_ ) \
{                                               \
  int i_;                                       \
                                                \
  for( i_=0; i_<(mstatptr_)->nctr; i_++ )       \
    {                                           \
      (sum_)+=((mstatptr_)->time[i_]);          \
    }                                           \
}




#define MSTAT_SUBTRACT( mstatp1_, mstatp2_ )   \
{                                              \
  int i_;                                      \
                                               \
  for( i_=0; (i_<(mstatp1_)->nexect) &&        \
             (i_<(mstatp2_)->nexect) ; i_++ )  \
    {                                          \
      ((mstatp1_)->exect[i_])-=                \
                 ((mstatp2_)->exect[i_]);      \
    }                                          \
}

#ifdef OMPP_USE_PAPI

#define MSTAT_SUBTRACT_CTRS( mstatp1_, mstatp2_ )   \
{                                                   \
  int i_, j_;                                       \
                                                    \
  for( j_=0; j_<OMPP_PAPI_MAX_CTRS; j_++ )          \
    {                                               \
     for( i_=0; (i_<(mstatp1_)->nctr) &&            \
                (i_<(mstatp2_)->nctr); i_++ )       \
       {                                            \
         ((mstatp1_)->ctr[j_][i_])-=                \
                  ((mstatp2_)->ctr[j_][i_]);        \
       }                                            \
    }                                               \
}

#else

#define MSTAT_SUBTRACT_CTRS( mstatp1_, mstatp2_ )  



#ifdef OMPP_MPI 

#define MSTAT_SUBTRACT_MPI( mstatp1_, mstatp2_ )          \
{                                                         \
  int i_;                                                 \
                                                          \
  for( i_=0; (i_<(mstatp1_)->nctr) &&                     \
             (i_<(mstatp2_)->nctr); i_++ )                \
  {                                                       \
     ((mstatp1_)->time[i_])-= ((mstatp2_)->time[i_]);     \
     ((mstatp1_)->nsend[i_])-=((mstatp2_)->nsend[i_]);    \
     ((mstatp1_)->nrecv[i_])-=((mstatp2_)->nrecv[i_]);    \
     ((mstatp1_)->ncoll[i_])-=((mstatp2_)->ncoll[i_]);    \
     ((mstatp1_)->bout[i_])-= ((mstatp2_)->bout[i_]);     \
     ((mstatp1_)->bin[i_])=-  ((mstatp2_)->bin[i_]);      \
  }                                                       \
}

#else 
#define MSTAT_SUBTRACT_MPI( mstatp1_, mstatp2_ ) /* EMPTY */
#endif


#endif /* OMPP_USE_PAPI */


#ifdef OMPP_USE_PAPI

#define MSTAT_ASSIGN( mstatp1_, mstatp2_ )     \
{                                              \
  int i_, j_;                                  \
                                               \
  for( i_=0; (i_<(mstatp1_)->nexect) &&        \
             (i_<(mstatp2_)->nexect); i_++ )   \
    {                                          \
      ((mstatp1_)->exect[i_])=                 \
                 ((mstatp2_)->exect[i_]);      \
    }                                          \
                                               \
  for( j_=0; j_<OMPP_PAPI_MAX_CTRS; j_++ )     \
    {                                          \
     for( i_=0; (i_<(mstatp1_)->nctr) &&       \
                (i_<(mstatp2_)->nctr) ; i_++ ) \
       {                                       \
         ((mstatp1_)->ctr[j_][i_])=            \
                  ((mstatp2_)->ctr[j_][i_]);   \
       }                                       \
    }                                          \
}

#else

#define MSTAT_ASSIGN( mstatp1_, mstatp2_ )     \
{                                              \
  int i_;                                      \
                                               \
  for( i_=0; (i_<(mstatp1_)->nexect) &&        \
             (i_<(mstatp2_)->nexect); i_++ )   \
    {                                          \
      ((mstatp1_)->exect[i_])=                 \
                 ((mstatp2_)->exect[i_]);      \
    }                                          \
}

#endif /* OMPP_USE_PAPI */

#ifdef OMPP_MPI

#define MSTAT_ASSIGN_MPI( mstatp1_, mstatp2_ )            \
{                                                         \
  int i_;                                                 \
                                                          \
  for( i_=0; (i_<(mstatp2_)->nctr); i_++ )                \
  {                                                       \
     ((mstatp1_)->time[i_])= ((mstatp2_)->time[i_]);      \
     ((mstatp1_)->nsend[i_])=((mstatp2_)->nsend[i_]);     \
     ((mstatp1_)->nrecv[i_])=((mstatp2_)->nrecv[i_]);     \
     ((mstatp1_)->ncoll[i_])=((mstatp2_)->ncoll[i_]);     \
     ((mstatp1_)->bout[i_])= ((mstatp2_)->bout[i_]);      \
     ((mstatp1_)->bin[i_])=  ((mstatp2_)->bin[i_]);       \
  }                                                       \
}

#endif 

#define MSTAT_SUMT( mstatptr_, sum_ )         \
{                                             \
  int i_;                                     \
  sum_=0.0;                                   \
                                              \
  for( i_=0; i_<(mstatptr_)->nexect; i_++ )   \
    {                                         \
      sum_+=(mstatptr_)->exect[i_];           \
    }                                         \
}

#ifdef OMPP_MPI

#define MSTAT_SUMT_MPI( mstatptr_, sum_ )     \
{                                             \
  int i_;                                     \
  sum_=0.0;                                   \
                                              \
  for( i_=0; i_<(mstatptr_)->nctr; i_++ )     \
    {                                         \
      sum_+=(mstatptr_)->time[i_];            \
    }                                         \
}
#endif 


#define MSTAT_SUMC( mstatptr_, sum_ )         \
{                                             \
  int i_;                                     \
  sum_=0;                                     \
                                              \
  for( i_=0; i_<(mstatptr_)->nexect; i_++ )   \
    {                                         \
      sum_+=(mstatptr_)->execc[i_];           \
    }                                         \
}

#define MSTAT_MINC( mstatptr_, min_ )         \
{                                             \
  int i_;                                     \
                                              \
  min_=(mstatptr_)->execc[0];                 \
  for( i_=1; i_<(mstatptr_)->nexect; i_++ )   \
    {                                         \
      if( min_>(mstatptr_)->execc[i_] )       \
        min_=(mstatptr_)->execc[i_];          \
    }                                         \
}

#define MSTAT_MAXC( mstatptr_, max_ )         \
{                                             \
  int i_;                                     \
                                              \
  max_=(mstatptr_)->execc[0];                 \
  for( i_=1; i_<(mstatptr_)->nexect; i_++ )   \
    {                                         \
      if( max_<(mstatptr_)->execc[i_] )       \
        max_=(mstatptr_)->execc[i_];          \
    }                                         \
}



// region statistics
typedef struct regstat
{
  // maximum number of threads
  unsigned nthreads;

  mstat_t  *enter;
  mstat_t  *exit;
  mstat_t  *main;
  mstat_t  *main_x; // for eXclusive times/counts
  mstat_t  *body;
  mstat_t  *body_x; // for eXclusive times/counts
  mstat_t  *ibarr;  // implicit barrier
  mstat_t  *seq;
  mstat_t  *task;   // task execution in implicit barriers

  struct ompp_region *reg;
} 
regstat_t;



extern void mstat_init( mstat_t *mstat,
			regstat_t *stat,
			unsigned nexecc, unsigned nexect,
			unsigned nctr );



extern void regstat_init( regstat_t *stat,
			  struct ompp_region *reg,
			  unsigned nthreads );


extern void regstat_on_enter( regstat_t *stat,
			      ompp_reg_type_t type,
			      unsigned tid, int rstack );


extern void regstat_on_exit( regstat_t *stat,
			     ompp_reg_type_t type,
			     unsigned tid, int rstack );



// raw dump of the region stats
void regstat_dump( regstat_t *stat, FILE *f );


// pretty-print the region stats
void regstat_print( FILE *f, regstat_t *stat, int excl );
void regstat_print_header( regstat_t *stat, FILE *f );

long long mstat_sumctr( mstat_t *mstat, unsigned c );
void mstat_enter( mstat_t *mstat, unsigned tid, perfdata_t pd );
void mstat_exit( mstat_t *mstat, unsigned tid, perfdata_t pd );

// get the summed wallclock execution time
double regstat_getcputime( regstat_t *stat );
double regstat_getcputime_x( regstat_t *stat );

double regstat_getwtime( regstat_t *stat );
double regstat_getwtime_x( regstat_t *stat );




#define PD_EXECT      (1)
#define PD_EXECT_X    (1<<1)
#define PD_EXECC      (1<<2)
#define PD_IBARRT     (1<<3)
#define PD_BODYT      (1<<4)
#define PD_BODYT_X    (1<<5)
#define PD_BODYC      (1<<6)
#define PD_ENTERT     (1<<7)
#define PD_ENTERC     (1<<8)
#define PD_EXITT      (1<<9)
#define PD_EXITC      (1<<10)
#define PD_CTR_B      (1<<11) 
#define PD_CTR_B_X    (1<<12) 
#define PD_CTR_M      (1<<13)
#define PD_CTR_M_X    (1<<14)
#define PD_MGMTT      (1<<15) 
#define PD_MPI_TIME   (1<<16)
#define PD_MPI_NSEND  (1<<17)
#define PD_MPI_NRECV  (1<<18)
#define PD_MPI_NCOLL  (1<<19)
#define PD_MPI_BOUT   (1<<20)
#define PD_MPI_BIN    (1<<21)
#define PD_TASKT      (1<<22)


typedef struct pline
{
  double    exect;
  double    exect_x;
  unsigned  execc;
  double    ibarrt;
  double    bodyt;
  double    bodyt_x;
  unsigned  bodyc;
  double    entert;
  unsigned  enterc;
  double    exitt;
  unsigned  exitc;
  double    taskt;

#ifdef OMPP_MPI
  ompp_mpi_t mpi;
#endif /* OMPP_MPI */

#ifdef OMPP_USE_PAPI
  long long ctr[OMPP_PAPI_MAX_CTRS];
  long long ctr_x[OMPP_PAPI_MAX_CTRS];
#endif
  
  unsigned  selector;
} pline_t;


void pline_print( FILE* f, int tid, pline_t *pd );
void pline_addto( pline_t *pd1, pline_t *pd2 );
void pline_init( pline_t *pd );
void mstat_dump( mstat_t *mstat, FILE *f );

#endif /* OMPP_REGSTAT_H_INCLUDED */
