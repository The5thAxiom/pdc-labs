/*
 * ompp_rstack.c  */
/* @OMPP_HEADER@ */

#include <stdio.h>
#include <assert.h>
#include <omp.h>

#include "ompp.h"
#include "ompp_region.h"
#include "ompp_rstack.h"
#include "ompp_regstat.h"

omp_lock_t rstack_lock;

void rnode_init(rnode_t *node)
{
  int i, j;
  static unsigned id=0;

  node->id=id++;
  node->reg=0;
  node->next=0;
  node->child=0;
  node->parent=0;
  node->rstatnum=0;
  node->data=0;

  node->rstat = (struct regstat*) calloc( 1, sizeof(regstat_t) );
  assert( node->rstat );

#ifdef OMPP_CONTROLFLOW 
  /* experimental */
  node->predcount = (int**) malloc( sizeof(int*)*ompp_max_threads);
  for( i=0; i<ompp_max_threads; i++ )
    {
      node->predcount[i]=(int*) malloc( sizeof(int)*OMPP_MAX_REGIONS );
      for( j=0; j<OMPP_MAX_REGIONS; j++ )
	{
	  node->predcount[i][j]=0;
	}
    }
#endif /* OMPP_CONTROLFLOW */
  
}

void rstack_init(rstack_t *st, size_t maxth )
{
  int i;

  omp_init_lock( &rstack_lock );

  /* initialize the root node  */
  (st->root).reg    = 0;
  (st->root).next   = 0;
  (st->root).child  = 0;
  (st->root).parent = 0;
  
  st->current  = (rnode_t**) calloc( maxth, sizeof(rnode_t*) );
#ifdef OMPP_CONTROLFLOW
  st->previous = (rnode_t**) calloc( maxth, sizeof(rnode_t*) );
#endif /* OMPP_CONTROLFLOW */
  st->maxthreads = maxth;

  /* set the current pointer to root for all threads */
  for( i=0; i<maxth; i++ )
    {
      st->current[i]=&(st->root);
#ifdef OMPP_CONTROLFLOW
      st->previous[i]=0;
#endif /* OMPP_CONTROLFLOW */
    }
}


void rstack_enter(rstack_t *st, struct ompp_region *reg, int tid) 
{
  /* current and new node */
  rnode_t *cnode, *nnode;
  rnode_t *tmp, *last;

  cnode = (st->current)[tid];
  assert( cnode );
  
  /*
  fprintf(stderr, "STACK [%d] enter %s %s %p\n",
	  tid, ompp_region_full_name[reg->type],
	  reg->location,
	  reg);
  */
 
  omp_set_lock( &rstack_lock );

  /* recursive entry ? */
  if( (st->current)[tid]->reg == reg )
    {
      /* XXX do nothing ? */

      omp_unset_lock( &rstack_lock );
      return;
    }


  /* no child present -> make new node and store it as child */
  if( !(cnode->child) )
    {
      /* about to modify...set lock and check again */
      /* omp_set_lock( &rstack_lock ); */
      if( !(cnode->child) )
	{
	  nnode = (rnode_t*) calloc( 1, sizeof(rnode_t) );
	  rnode_init( nnode );
	  
	  nnode->reg      = reg;
	  nnode->rstatnum = (reg->nstats)++;
	  cnode->child    = nnode;
	  nnode->parent   = cnode;
	  

	  regstat_init( nnode->rstat, reg, 
			ompp_max_threads );

#ifdef OMPP_CONTROLFLOW
	  if( (st->previous)[tid] && (st->previous)[tid]->parent == (st->current)[tid]  )
	    {
	      /* do nothing */
	    }
	  else
	    {
	      (st->previous)[tid] = (st->current)[tid];
	    }
#endif /* OMPP_CONTROLFLOW */


	  /* make new node the current one */
	  (st->current)[tid] = nnode;
      
	  
	  /* omp_unset_lock( &rstack_lock ); */
	  omp_unset_lock( &rstack_lock );
	  return;
	}
      else 
	{
	  /* fprintf(stderr, " XXX this is a surprise (adding as child)\n"); */
	  /* omp_unset_lock( &rstack_lock ); */
	}
    }
  

  /* search on the child level */
  tmp=cnode->child; last=tmp;
  while( tmp && ((tmp->reg)!=reg) )
    {
      /* one behind -> last is always valid */
      last = tmp; 
      tmp = tmp->next;
    }

  if( tmp )
    {
#ifdef OMPP_CONTROLFLOW
      if( (st->previous)[tid] && (st->previous)[tid]->parent == (st->current)[tid]  )
	{
	  /* do nothing */
	}
      else
	{
	  (st->previous)[tid] = (st->current)[tid];
	}
#endif
      /* found on the cild level, nothing to do... */
      (st->current)[tid] = tmp;
    }
  else /* OMPP_CONTROLFLOW */
    { 
      /* not found... lock and search again */
      /* omp_set_lock( &rstack_lock ); */
      
      tmp=cnode->child; last=tmp;
      while( tmp && ((tmp->reg)!=reg) )
	{
	  /* one behind -> last is always valid */
	  last = tmp; 
	  tmp = tmp->next;
	}
      
      if( tmp )
	{
#ifdef OMPP_CONTROLFLOW
	  if( (st->previous)[tid] && (st->previous)[tid]->parent == (st->current)[tid]  )
	    {
	      /* do nothing */
	    }
	  else
	    {
	      /* found on the child level, nothing to do... */
	      (st->previous)[tid] = (st->current)[tid];
	    }
#endif /* OMPP_CONTROLFLOW */
	  (st->current)[tid] = tmp;
	  
	}
      else
	{
	  /* make new node and add it at head */
	  nnode = (rnode_t*) calloc( 1, sizeof(rnode_t) );
	  rnode_init( nnode );
	  
	  nnode->reg      = reg;
	  nnode->rstatnum = (reg->nstats)++;
	  nnode->parent   = cnode;
	  last->next      = nnode;
	  
	  regstat_init( nnode->rstat, reg, 
			ompp_max_threads );

#ifdef OMPP_CONTROLFLOW 
	  if( (st->previous)[tid] && (st->previous)[tid]->parent == (st->current)[tid]  )
	    {
	      /* do nothing */
	    }
	  else
	    {
	      (st->previous)[tid] = (st->current)[tid];
	    }
#endif /* OMPP_CONTROLFLOW */

	  (st->current)[tid] = nnode;
	}
      
      /* omp_unset_lock( &rstack_lock ); */
    }

  omp_unset_lock( &rstack_lock );
}


void rstack_exit(rstack_t *st, struct ompp_region *reg, int tid) 
{
  /* current node */
  rnode_t *cnode;

  /*
  fprintf(stderr, "STACK [%d] exit %s %s %p\n",  
	  tid, ompp_region_full_name[reg->type],
	  reg->location, reg );
  */
 

  omp_set_lock( &rstack_lock );

  cnode = (st->current)[tid];
  assert( cnode );

  if( (cnode->reg==reg) && (cnode->parent) )
    {
      /* go to parent if it is set */
#ifdef OMPP_CONTROLFLOW
      (st->previous)[tid] = (st->current)[tid];
#endif /* OMPP_CONTROLFLOW */
      (st->current)[tid]=cnode->parent;
    }
  /*
  else
    {
      if( !(cnode->parent) )
	fprintf(stderr, "parent node not set\n");
      
      if( (cnode->reg!=reg) ) 
	fprintf(stderr, "not same region\n");
    }
  */

  omp_unset_lock( &rstack_lock );
}

void rstack_print_node( rnode_t *node, int indent )
{
  int i;
  for( i=0; i<indent; i++ )
    {
      fprintf(stderr, " ");
    }

  if( node->reg )
    {
      fprintf(stderr, " [%s %s]\n",
	      ompp_region_full_name[node->reg->type],
	      node->reg->location );

      regstat_print( stderr, node->rstat, 0 );
    }
  else
    {
      fprintf(stderr, "(null)\n");
      
    }
  
  if( node->child )
    rstack_print_node( node->child, indent+4 );
  
  if( node->next )
    rstack_print_node( node->next, indent );
}

void rstack_print(rstack_t *st)
{
  rstack_print_node( &(st->root), 0 );
}



/* XXX move to rstack.c */
void rstack_compute_xstats( rnode_t *node )
{
  regstat_t *rstat;
  rnode_t *tmp;

  assert( node );   
  rstat = node->rstat;
 
  if( !rstat ) return;

  if( rstat->body_x )
    {
      MSTAT_ASSIGN( rstat->body_x, rstat->body );
#ifdef OMPP_MPI
      MSTAT_ASSIGN_MPI(  rstat->body_x, rstat->body );
#endif
    }

  if( rstat->main_x )
    {
      MSTAT_ASSIGN( rstat->main_x, rstat->main );
#ifdef OMPP_MPI
      MSTAT_ASSIGN_MPI( rstat->main_x, rstat->main );
#endif
    }
  
  for( tmp=node->child; tmp; tmp=tmp->next )
    {
      assert(tmp);
      assert(tmp->rstat);
      assert(tmp->rstat->main);

      if( tmp->reg && tmp->reg->type==OMPP_TASKEXEC )
	continue;
      
      if( rstat->main_x )
	{
	  assert( !(rstat->body_x) );

	  MSTAT_SUBTRACT( rstat->main_x, tmp->rstat->main );

	  if( tmp->rstat->main_x )
	    {
	      assert( !(tmp->rstat->body_x) );
	      MSTAT_SUBTRACT_CTRS( rstat->main_x, tmp->rstat->main );
#ifdef OMPP_MPI
	      MSTAT_SUBTRACT_MPI( rstat->main_x, tmp->rstat->main );
#endif
	    }
	  if( tmp->rstat->body_x )
	    {
	      assert( !(tmp->rstat->main_x) );
	      MSTAT_SUBTRACT_CTRS( rstat->main_x, tmp->rstat->body );
#ifdef OMPP_MPI
	      MSTAT_SUBTRACT_MPI( rstat->main_x, tmp->rstat->body );
#endif
	    }
	}
      
      if( rstat->body_x )
	{
	  assert( !(rstat->main_x) );

	  MSTAT_SUBTRACT( rstat->body_x, tmp->rstat->main );

#ifdef OMPP_MPI
	  MSTAT_SUBTRACT_MPI( rstat->body_x, tmp->rstat->main );
#endif

	  if( tmp->rstat->main_x )
	    {
	      assert( !(tmp->rstat->body_x) );
	      MSTAT_SUBTRACT_CTRS( rstat->body_x, tmp->rstat->main );
#ifdef OMPP_MPI
	      MSTAT_SUBTRACT_MPI( rstat->body_x, tmp->rstat->main );
#endif
	    }
	  if( tmp->rstat->body_x )
	    {
	      assert( !(tmp->rstat->main_x) );
	      MSTAT_SUBTRACT_CTRS( rstat->body_x, tmp->rstat->body );
#ifdef OMPP_MPI
	      MSTAT_SUBTRACT_MPI( rstat->body_x, tmp->rstat->body );
#endif
	    }
	}
    }

  /*
  fprintf(stderr, "*** compute_xstats %s\n",
	  ompp_region_full_name[node->reg->type]);
  */
}
