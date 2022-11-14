/*
 * ompp_rstack.h  */
/* @OMPP_HEADER@ */


#ifndef OMPP_RSTACK_H_INCLUDED
#define OMPP_RSTACK_H_INCLUDED

#include <stddef.h>
#include <omp.h>

#define OMPP_MAX_REGIONS  (800)

/* #include "ompp_regstat.h" */

extern omp_lock_t rstack_lock;

struct regstat;



/* region stack node */
struct rnode
{
  unsigned id;

  /* number of threads that execute the region */
  int nthreads;

  /* pointer to the region */
  struct ompp_region *reg;
  
  struct rnode *next;    /* next at same level */
  struct rnode *child;   /* first at sublevel */

  struct rnode *parent;  /* parent node */

  struct regstat *rstat;

  void *data;
  
  /* the rstat above is the rstatnum copy 
     for this region */
  int rstatnum;
  

#ifdef OMPP_CONTROLFLOW
  /* experimental */
  int **predcount;
#endif /* OMPP_CONTROLFLOW */
};

typedef struct rnode rnode_t;


/* region stack */
struct rstack
{
  /* maximum number of threads */
  size_t maxthreads;

  /* root node of the region stack */
  struct rnode root;

  /* points to the current rstack node 
     for each thread  */
  struct rnode **current;

#ifdef OMPP_CONTROLFLOW
  /* experimental */
  struct rnode **previous;
#endif /* OMPP_CONTROLFLOW */
};

typedef struct rstack rstack_t;


void rnode_init(rnode_t *node);

void rstack_init(rstack_t *st, size_t maxth);

void rstack_enter(rstack_t *st, struct ompp_region *reg, int tid);
void rstack_exit(rstack_t *st, struct ompp_region *reg, int tid);

void rstack_print_node( rnode_t *node, int indent );
void rstack_print(rstack_t *st);

void rstack_compute_xstats( rnode_t *node );



#endif /* OMPP_RSTACK_H_INCLUDED */
