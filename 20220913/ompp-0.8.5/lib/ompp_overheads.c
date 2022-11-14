/*
 * ompp_overheads.c  */
/* @OMPP_HEADER@ */


#include <assert.h>

#include "ompp_rstack.h"
#include "ompp_overheads.h"
#include "ompp_region.h"
#include "utils/list.h"
#include "ompp_regstat.h"
#include "ompp_report_callgraph.h"

void overheads_init( overheads_t *ovhds )
{
  int i;

  ovhds->reg     = 0;
  
  ovhds->sync    = 0.0;
  ovhds->imbal   = 0.0;
  ovhds->limpar  = 0.0;
  ovhds->mgmt    = 0.0;
  ovhds->sumovhd = 0.0;
  ovhds->wallt   = 0.0;

#ifdef OMPP_MPI
  ovhds->mpit    = 0.0;
#endif

#ifdef OMPP_USE_PAPI
  for( i=0; i<OMPP_PAPI_MAX_CTRS; i++ )
    {
      ovhds->ctr[i] = 0;
    }
#endif
}


int ovhd_compare_by_wallclock( const void *p1, const void *p2 )
{
    overheads_t *ovhd1 = (overheads_t*)p1;
    overheads_t *ovhd2 = (overheads_t*)p2;
    
    return ((ovhd1->wallt) < (ovhd2->wallt));
}


int ovhd_compare_by_overheads( const void *p1, const void *p2 )
{
    overheads_t *ovhd1 = (overheads_t*)p1;
    overheads_t *ovhd2 = (overheads_t*)p2;
    
    return ((ovhd1->sumovhd) < (ovhd2->sumovhd));
}



void attribute_overheads( overheads_t *ovhd, 
			  ompp_region_t *reg, 
			  regstat_t *stat,
			  rnode_t *node )
{
  double d;
  rnode_t *tmp;


  switch( reg->type )
    {
    case OMPP_FLUSH:
    case OMPP_ATOMIC:
      //OLD ovhds[i].sync += mstat_sumt( stat->main );
      MSTAT_SUMT_ADDTO( stat->main, ovhd->sync );
      break;
      
    case OMPP_BARRIER:
      //OLD ovhds[i].sync += mstat_sumt( stat->main );
      MSTAT_SUMT_ADDTO( (stat->main), (ovhd->sync) );
      break;
      
    case OMPP_CRITICAL:
      //OLD ovhds[i].sync += mstat_sumt( stat->enter );
      MSTAT_SUMT_ADDTO( stat->enter, ovhd->sync );
      
      //OLD ovhds[i].mgmt += mstat_sumt( stat->exit );
      MSTAT_SUMT_ADDTO( stat->exit, ovhd->mgmt );
      break;
      
    case OMPP_WORKSHARE:
    case OMPP_LOOP:
      //OLD ovhds[i].imbal += mstat_sumt( stat->ibarr );
      MSTAT_SUMT_ADDTO( stat->ibarr, ovhd->imbal );
      break;
      
    case OMPP_MASTER:
      // ?? TODO: master only has seq
      break;
      
    case OMPP_PARALLEL_WORKSHARE:
    case OMPP_PARALLEL_LOOP:
    case OMPP_PARALLEL:
      //OLD ovhds[i].imbal += mstat_sumt( stat->ibarr );
      MSTAT_SUMT_ADDTO( stat->ibarr, ovhd->imbal );
      
      // XXX this startup / teardown overhead should actually be 
      // control of parallelism overhead ?!
      
      //OLD ovhds[i].mgmt += mstat_sumt( stat->enter );
      MSTAT_SUMT_ADDTO( stat->enter, ovhd->mgmt );
      
      //OLD ovhds[i].mgmt += mstat_sumt( stat->exit );
      MSTAT_SUMT_ADDTO( stat->exit, ovhd->mgmt );
      
#ifdef OMPP_USE_PAPI
      // FIXME
      //      for( j=0; j<OMPP_PAPI_MAX_CTRS; j++ )
      //	{
      //	  ovhds[i].ctr[j] += mstat_sumctr( stat->body, j);
      //	}
#endif 
#ifdef OMPP_MPI
      MSTAT_SUMT_ADDTO_MPI( stat->body, ovhd->mpit );
#endif
      break;
      
    case OMPP_SECTIONS:
      {
	//OLD unsigned bodyc = mstat_sumc( stat->body );
	//OLD unsigned mainc = mstat_sumc( stat->main );
	
	unsigned bodyc, mainc;
	double maint, bodyt, ibarrt;
	
	MSTAT_SUMC( stat->body, bodyc );
	MSTAT_SUMC( stat->main, mainc );
	
	MSTAT_SUMT( stat->body, bodyt);
	MSTAT_SUMT( stat->ibarr, ibarrt );
	MSTAT_SUMT( stat->main, maint );
	
	ovhd->mgmt+=maint-bodyt-ibarrt; 
	
	if( bodyc < mainc )
	  {
	    //OLD ovhds[i].limpar += mstat_sumt( stat->ibarr );
	    MSTAT_SUMT_ADDTO( stat->ibarr, ovhd->limpar );
	  }
	else
	  {
	    //OLD ovhds[i].imbal  += mstat_sumt( stat->ibarr );
	    MSTAT_SUMT_ADDTO( stat->ibarr, ovhd->imbal );
	  }
      }
      break;
      
    case OMPP_SINGLE:
      //OLD ovhds[i].limpar += mstat_sumt( stat->ibarr );
      MSTAT_SUMT_ADDTO( stat->ibarr, ovhd->limpar );
      break;
      
    case OMPP_REGION:
      // ??
      break;
      
    case OMPP_LOCK:
      //OLD ovhds[i].sync += mstat_sumt( stat->enter );
      MSTAT_SUMT_ADDTO( stat->enter, ovhd->sync );
      
      //OLD ovhds[i].mgmt += mstat_sumt( stat->exit );
      MSTAT_SUMT_ADDTO( stat->exit, ovhd->mgmt );
      break;
      
      
    case OMPP_PARALLEL_SECTIONS:
      {
	//OLD unsigned bodyc = mstat_sumc( stat->body );
	//OLD unsigned mainc = mstat_sumc( stat->main );
	unsigned bodyc, mainc;
	MSTAT_SUMC( stat->body, bodyc );
	MSTAT_SUMC( stat->main, mainc );
	
	if( bodyc < mainc )
	  {
	    //OLD ovhds[i].limpar += mstat_sumt( stat->ibarr );
	    MSTAT_SUMT_ADDTO( stat->ibarr, ovhd->limpar );
	  }
	else
	  {
	    //OLD ovhds[i].imbal += mstat_sumt( stat->ibarr );
	    MSTAT_SUMT_ADDTO( stat->ibarr, ovhd->imbal );
	  }
	
	//OLD ovhds[i].mgmt += mstat_sumt( stat->enter );
	MSTAT_SUMT_ADDTO( stat->enter, ovhd->mgmt );
	
	//OLD ovhds[i].mgmt += mstat_sumt( stat->exit );
	MSTAT_SUMT_ADDTO( stat->exit, ovhd->mgmt );
	
#ifdef OMPP_USE_PAPI

	// FIXME
	// for( j=0; j<OMPP_PAPI_MAX_CTRS; j++ )
	// {
	// ovhds[i].ctr[j] += mstat_sumctr( stat->body, j);
	// }
#endif
#ifdef OMPP_MPI
	MSTAT_SUMT_ADDTO_MPI( stat->body, ovhd->mpit );
#endif
      }
      break;

    case OMPP_TASKEXEC:
      d=0.0;
      MSTAT_SUMT( stat->main, d );
      
      tmp = node;
      while( tmp )
	{
	  if( tmp->reg && tmp->reg->type==OMPP_TASKWAIT ) {
	    break;
	  }
	
	  // explicit barrier
	  if( tmp->reg && tmp->reg->type==OMPP_BARRIER ) {
	    ovhd->sync-=d;
	    break;
	  }
	  if( tmp->reg && (tmp->reg->type==OMPP_PARALLEL ||
			   tmp->reg->type==OMPP_PARALLEL_LOOP ||
			   tmp->reg->type==OMPP_PARALLEL_WORKSHARE )) {
	    ovhd->imbal-=d;
	    break;
	  }
	  if( tmp->reg && (tmp->reg->type==OMPP_SINGLE ||
			   tmp->reg->type==OMPP_PARALLEL_SECTIONS) ) {
	    ovhd->limpar-=d;
	    break;
	  }
	  
	  tmp=tmp->parent;
	}
      
      //fprintf(stderr, "*** %f\n", d);
      break;
    }
  

}



// takes a list of regions and computes a list of 
// overead constructs (one for each parallel region 
//
void analyze_overheads( dllist_t *reglist, rstack_t *st,
			overheads_t *ovhds, unsigned npara )
{
  int i;
  rnode_t *cpath[OMPP_MAX_CALLGRAPH_DEPTH+1];
  unsigned level;
  rnode_t *node;
  ompp_region_t *par;  

  level=0;
  node=&(st->root);
  par=0;
  
  while(node)
    {
      cpath[level]=node;
      cpath[level+1]=0;

      par=0;
      for( i=0; i<=level; i++ )
	{
	  if( cpath[i] && cpath[i]->reg ) {
	    if( cpath[i]->reg->type==OMPP_PARALLEL ||
		cpath[i]->reg->type==OMPP_PARALLEL_LOOP ||
		cpath[i]->reg->type==OMPP_PARALLEL_WORKSHARE ||
		cpath[i]->reg->type==OMPP_PARALLEL_SECTIONS )
	      {
		par=cpath[i]->reg;
	      }
	  }
	}
      
      if( (node->reg) && (node->rstat) )
	{
	  for( i=0; i<npara; i++ )
	    {
	      if( ovhds[i].reg == par ) {
		// found -> ovds[i]
		break;
	      }
	      if( !(ovhds[i].reg) ) {
		// empty -> we take it -> ovhds[i]
		ovhds[i].reg =  par;
		break;
	      }
	    }
	  attribute_overheads( &(ovhds[i]), node->reg, node->rstat, node );
	}
      
      // advance to next node (depth-first)
      if( node->child ) 
	{
	  level++;
	  node=node->child;
	}
      else if( node->next )
	{
	  // levl unchanged!
	  node=node->next;
	    }
      else
	{
	  // back-tracking...
	  while(1)
	    {
	      node=node->parent;
	      level--;
	      
	      // note that if (node->parent)==0
	      // we are at the root node and we
	      // break out of the inner loop
	      // note that this will bail us out 
	      // of the outer while loop as well
	      // and the whole thing is finished
	      if( !node ) 
		break;
	      
	      if( node->next ) 
		{
		  // levl unchanged!
		  node=node->next;
		  break;
		}
	    }  
	}
    }
  
  for( i=0; i<npara; i++ )
    {
      // sum-up overheads
      ovhds[i].sumovhd = 
#ifdef OMPP_MPI
	ovhds[i].mpit +
#endif
	ovhds[i].sync +
	ovhds[i].imbal +
	ovhds[i].limpar +
	ovhds[i].mgmt;
      
      assert( ovhds[i].reg );
      assert( ovhds[i].reg->rstat );
      assert( ovhds[i].reg->rstat->seq );
      
      ovhds[i].wallt = ovhds[i].reg->rstat->seq->exect[0];
    }

}

