

#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "utils/list.h"
#include "ompp_rstack.h"
#include "ompp_region.h"
#include "ompp_report_cube.h"

#include "cubew/cube.h"


cube_region *ompp_cube_regs[OMPP_CUBE_MAX_REGIONS];
cube_metric *ompp_cube_mets[OMPP_CUBE_NUM_METRICS];
cube_thread **ompp_cube_threads;
  

void ompp_traverse_calltree_cube(cube_t *cube, rstack_t *st, unsigned select);
void ompp_cube3_handle_node(cube_t *cube, rnode_t *node, unsigned select);
void ompp_cube3_set_sev(cube_t *cube, cube_cnode *cnode, rnode_t *node);
void ompp_cube3_set_sev_as(cube_t *cube, cube_cnode *cnode, 
			   mstat_t *mstat, unsigned select);

void ompp_cube3_define_metrics(cube_t *cube);
void ompp_cube3_define_system(cube_t *cube);
void ompp_cube3_define_regions(cube_t *cube, dllist_t *rlist);
void ompp_cube3_define_cnodes(cube_t *cube, rstack_t *rstack);
void ompp_cube3_set_metrics(cube_t *, rstack_t *rstack);

void ompp_report_cube3( FILE* f, 
			dllist_t *rlist,
			rstack_t *rstack )
{
  int i;
  cube_t* cube;
  cube = cube_create();

  ompp_cube_threads = (cube_thread**) calloc( ompp_max_threads, sizeof(cube_thread*) );
  for( i=0; i<OMPP_CUBE_MAX_REGIONS; i++ )
    ompp_cube_regs[i]=0;
  for( i=0; i<OMPP_CUBE_NUM_METRICS; i++ )
    ompp_cube_mets[i]=0;


  ompp_cube3_define_metrics(cube);
  ompp_cube3_define_system(cube);
  ompp_cube3_define_regions(cube, rlist);
  ompp_cube3_define_cnodes(cube, rstack);
  cube_write_def(cube, f);

  ompp_cube3_set_metrics(cube, rstack);
  cube_write_sev_matrix(cube, f);
  //  cube_write_finish(cube,f);

  cube_free(cube);
}


void ompp_cube3_define_metrics(cube_t* cube)
{
  unsigned i;

  ompp_cube_mets[OMPP_CUBE_MET_TIME] =
    cube_def_met(cube, "Time", "", "", "sec", "",
		 "", "", NULL);

  ompp_cube_mets[OMPP_CUBE_MET_VISITS] =
    cube_def_met(cube, "Visits", "", "", "sec", "",
		 "", "", NULL);

  ompp_cube_mets[OMPP_CUBE_MET_WORK] =
    cube_def_met(cube, "Work", "", "", "sec", "",
		 "", "", ompp_cube_mets[OMPP_CUBE_MET_TIME]);
  
  ompp_cube_mets[OMPP_CUBE_MET_OVHDS] =
    cube_def_met(cube, "Overheads", "", "", "sec", "",
		 "", "", ompp_cube_mets[OMPP_CUBE_MET_TIME]);
  
  ompp_cube_mets[OMPP_CUBE_MET_OVHD_IMBAL] =
    cube_def_met(cube, "Load Imbalance", "", "", "sec", "",
		 "", "", ompp_cube_mets[OMPP_CUBE_MET_OVHDS]);

  ompp_cube_mets[OMPP_CUBE_MET_OVHD_LIMPAR] =
    cube_def_met(cube, "Limited Parallelism", "", "", "sec", "",
		 "", "", ompp_cube_mets[OMPP_CUBE_MET_OVHDS]);
		 
  ompp_cube_mets[OMPP_CUBE_MET_OVHD_SYNC] =
    cube_def_met(cube, "Synchronization", "", "", "sec", "",
		 "", "", ompp_cube_mets[OMPP_CUBE_MET_OVHDS]);
  
  ompp_cube_mets[OMPP_CUBE_MET_OVHD_MGMT] =
    cube_def_met(cube, "Thread Mgmt", "", "", "sec", "",
		 "", "", ompp_cube_mets[OMPP_CUBE_MET_OVHDS]);
  
#ifdef OMPP_MPI
  ompp_cube_mets[OMPP_CUBE_MET_MPITIME] =
    cube_def_met(cube, "MPI Time", "", "", "sec", "",
		 "", "", ompp_cube_mets[OMPP_CUBE_MET_OVHDS]);
		 
  ompp_cube_mets[OMPP_CUBE_MET_MPIDATAVOL] =
    cube_def_met(cube, "MPI Volume", "", "", "Bytes", "",
		 "", "", NULL );

  ompp_cube_mets[OMPP_CUBE_MET_MPIDATAIN] =
    cube_def_met(cube, "In", "", "", "Bytes", "",
		 "", "", ompp_cube_mets[OMPP_CUBE_MET_MPIDATAVOL]);

  ompp_cube_mets[OMPP_CUBE_MET_MPIDATAOUT] =
    cube_def_met(cube, "Out", "", "", "Bytes", "",
		 "", "", ompp_cube_mets[OMPP_CUBE_MET_MPIDATAVOL]);	 

  ompp_cube_mets[OMPP_CUBE_MET_MPINCALLS] =
    cube_def_met(cube, "MPI calls", "", "", "Count", "",
		 "", "", NULL);

  ompp_cube_mets[OMPP_CUBE_MET_MPINSEND] =
    cube_def_met(cube, "Send", "", "", "Count", "",
		 "", "", ompp_cube_mets[OMPP_CUBE_MET_MPINCALLS]);

  ompp_cube_mets[OMPP_CUBE_MET_MPINRECV] =
    cube_def_met(cube, "Recv", "", "", "Count", "",
		 "", "", ompp_cube_mets[OMPP_CUBE_MET_MPINCALLS]);

  ompp_cube_mets[OMPP_CUBE_MET_MPINCOLL] =
    cube_def_met(cube, "Collective", "", "", "Count", "",
		 "", "", ompp_cube_mets[OMPP_CUBE_MET_MPINCALLS]);

#endif /* OMPP_MPI */
  
}



void ompp_cube3_define_system(cube_t* cube)
{
  int i;
  char buf[80];

  cube_machine*  mach;
  cube_node*     node;
  cube_process*  proc;

  gethostname(buf, 80);

  mach  = cube_def_mach(cube, "Machine", "");
  node  = cube_def_node(cube, buf, mach);

#ifdef OMPP_MPI
  sprintf(buf, "Process %d", ompp_mpi_myrank);
  proc = cube_def_proc(cube, buf, ompp_mpi_myrank, node);
#else
  proc = cube_def_proc(cube, "Process", 0, node);
#endif /* OMPP_MPI */
  
  for( i=0; i<ompp_max_threads; i++ )
    {
      sprintf(buf, "Thread %d", i);
      ompp_cube_threads[i] = cube_def_thrd(cube, buf, i, proc);
    }
}


void ompp_cube3_define_regions(cube_t *cube, dllist_t *rlist)
{
  lelem_t *elem; 
  ompp_region_t *reg;
  unsigned rid;
  char buf[80];
  
  elem=rlist->head;
  while( elem )
    {
      reg = (ompp_region_t*)elem->data;

      sprintf(buf, "%s %s", ompp_region_full_name[reg->type], reg->location );

      rid = reg->number;
      if( reg->desc )
	{
	  ompp_cube_regs[rid]=
	    cube_def_region(cube, buf, 
			    reg->desc->begin_first_line,
			    reg->desc->end_first_line, 
			    "", "", 
			    reg->desc->file_name);
	}

      elem=elem->next;
    }

  
  ompp_cube_regs[0]=
    cube_def_region(cube, "APPLICATION",  0, 0, "", "", "" );
}



void ompp_traverse_calltree_cube(cube_t *cube, rstack_t *st, unsigned select)
{
  int levl;
  rnode_t *node;
  
  /* we need this as the termination criterion later */
  assert( (st->root).parent==0 );

  node=&(st->root);
  while(node)
    {
      /* do something with the node */

      if( select&OMPP_CUBE_SET_METRICS )
	rstack_compute_xstats(node);
      
      ompp_cube3_handle_node(cube, node, select);
      
      /* advance to next node (depth-first) */
      if( node->child ) 
	{
	  node=node->child;
	}
      else if( node->next )
	{
	  /* levl unchanged! */
	  node=node->next;
	}
      else
	{
	  /* back-tracking... */
	  while(1)
	    {
	      node=node->parent;
	      levl--;
	     
	      /* note that if (node->parent)==0
		 we are at the root node and we
		 break out of the inner loop
		 note that this will bail us out 
		 of the outer while loop as well
		 and the whole thing is finished  */

	      if( !node ) 
		break;

	      if( node->next ) 
		{
		  /* levl unchanged! */
		  node=node->next;
		  break;
		}
	    }  
	}
    }
}


void ompp_cube3_define_cnodes(cube_t *cube, rstack_t *rstack)
{
  ompp_traverse_calltree_cube(cube, rstack, OMPP_CUBE_DEFINE_CNODES);
}

void ompp_cube3_set_metrics(cube_t *cube, rstack_t *rstack)
{
  ompp_traverse_calltree_cube(cube, rstack, OMPP_CUBE_SET_METRICS);
}


void ompp_cube3_handle_node(cube_t *cube, rnode_t *node, unsigned select)
{
  cube_cnode *cnodep, *cnode; 

  cnodep = (node->parent?(cube_cnode*)(node->parent->data):(cube_cnode*)0);  
  cnode  = (cube_cnode*)(node->data);

  if( select&OMPP_CUBE_DEFINE_CNODES )
    {
      if( !(node->reg) ) { /* root node */
	node->data = cnode =
	  cube_def_cnode(cube, ompp_cube_regs[0], 0 );
      }
      else {
	cnode = cube_def_cnode(cube, 
			       ompp_cube_regs[node->reg->number], 
			       cnodep );
	node->data = cnode;
      }
    }
  
  if( select&OMPP_CUBE_SET_METRICS )
    {
      if( !(node->reg) ) { /* root node */
	cube_set_sev(cube, ompp_cube_mets[0], 
		     cnode, ompp_cube_threads[0], 1.0);
      }
      else {

	    ompp_cube3_set_sev(cube, cnode, node);


      }      
    }
}


void ompp_cube3_set_sev(cube_t *cube, cube_cnode *cnode, rnode_t *node)
{
  ompp_region_t *reg;
  regstat_t *rstat;
  int i;

  reg=node->reg;
  rstat=node->rstat;
  
  if( !reg || !rstat )
    return;

  switch( reg->type )
    {
    case OMPP_MASTER:
    case OMPP_REGION:
    case OMPP_TASK:
    case OMPP_TASKEXEC:
      ompp_cube3_set_sev_as(cube, cnode, rstat->main_x, 
			    OMPP_CUBE_MET_WORK);
      ompp_cube3_set_sev_as(cube, cnode, rstat->main, 
			    OMPP_CUBE_MET_VISITS);

#ifdef OMPP_MPI
      ompp_cube3_set_sev_as(cube, cnode, rstat->main_x, 
			    OMPP_CUBE_MET_MPITIME);
      ompp_cube3_set_sev_as(cube, cnode, rstat->main_x, 
			    OMPP_CUBE_MET_MPIDATAIN);
      ompp_cube3_set_sev_as(cube, cnode, rstat->main_x, 
			    OMPP_CUBE_MET_MPIDATAOUT);
      ompp_cube3_set_sev_as(cube, cnode, rstat->main_x, 
			    OMPP_CUBE_MET_MPINSEND);
      ompp_cube3_set_sev_as(cube, cnode, rstat->main_x, 
			    OMPP_CUBE_MET_MPINRECV);
      ompp_cube3_set_sev_as(cube, cnode, rstat->main_x, 
			    OMPP_CUBE_MET_MPINCOLL);
#endif /* OMPP_MPI */

      break;

    case OMPP_ATOMIC:
    case OMPP_BARRIER:
    case OMPP_FLUSH:
    case OMPP_TASKWAIT:
      ompp_cube3_set_sev_as(cube, cnode, rstat->main, 
			    OMPP_CUBE_MET_OVHD_SYNC);
      break;

    case OMPP_PARALLEL:
    case OMPP_PARALLEL_LOOP:
    case OMPP_PARALLEL_WORKSHARE:
    case OMPP_PARALLEL_SECTIONS:
      ompp_cube3_set_sev_as(cube, cnode, rstat->enter, 
			    OMPP_CUBE_MET_OVHD_MGMT);
      ompp_cube3_set_sev_as(cube, cnode, rstat->exit, 
			    OMPP_CUBE_MET_OVHD_MGMT);
      ompp_cube3_set_sev_as(cube, cnode, rstat->ibarr, 
			    OMPP_CUBE_MET_OVHD_IMBAL);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_WORK);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body,
			    OMPP_CUBE_MET_VISITS);

#ifdef OMPP_MPI
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_MPITIME);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x,
			    OMPP_CUBE_MET_MPIDATAIN);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_MPIDATAOUT);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_MPINSEND);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_MPINRECV);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_MPINCOLL);
#endif /* OMPP_MPI */


      break;

    case OMPP_LOOP:
    case OMPP_WORKSHARE:
      ompp_cube3_set_sev_as(cube, cnode, rstat->ibarr, 
			    OMPP_CUBE_MET_OVHD_IMBAL);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_WORK);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body,
			    OMPP_CUBE_MET_VISITS);

#ifdef OMPP_MPI
      ompp_cube3_set_sev_as(cube, cnode, rstat->body, 
			    OMPP_CUBE_MET_MPITIME);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body, 
			    OMPP_CUBE_MET_MPIDATAIN);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body, 
			    OMPP_CUBE_MET_MPIDATAOUT);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body, 
			    OMPP_CUBE_MET_MPINSEND);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body, 
			    OMPP_CUBE_MET_MPINRECV);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body, 
			    OMPP_CUBE_MET_MPINCOLL);
#endif /* OMPP_MPI */
      break;
      
    case OMPP_SECTIONS:
    case OMPP_SINGLE:
      ompp_cube3_set_sev_as(cube, cnode, rstat->ibarr, 
			    OMPP_CUBE_MET_OVHD_IMBAL);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_WORK);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body,
			    OMPP_CUBE_MET_VISITS);

#ifdef OMPP_MPI
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_MPITIME);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x,
			    OMPP_CUBE_MET_MPIDATAIN);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_MPIDATAOUT);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_MPINSEND);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_MPINRECV);

      ompp_cube3_set_sev_as(cube, cnode, rstat->body, 
			    OMPP_CUBE_MET_MPINCOLL);
#endif /* OMPP_MPI */

      break;

    case OMPP_CRITICAL:
    case OMPP_LOCK:
      ompp_cube3_set_sev_as(cube, cnode, rstat->enter, 
			    OMPP_CUBE_MET_OVHD_SYNC);
      ompp_cube3_set_sev_as(cube, cnode, rstat->exit, 
			    OMPP_CUBE_MET_OVHD_SYNC);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_WORK);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body,
			    OMPP_CUBE_MET_VISITS);
#ifdef OMPP_MPI
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_MPITIME);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x,
			    OMPP_CUBE_MET_MPIDATAIN);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_MPIDATAOUT);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_MPINSEND);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_MPINRECV);
      ompp_cube3_set_sev_as(cube, cnode, rstat->body_x, 
			    OMPP_CUBE_MET_MPINCOLL);
#endif /* OMPP_MPI */

      break;
    }

}


void ompp_cube3_set_sev_as(cube_t *cube, cube_cnode *cnode, 
			   mstat_t *mstat, unsigned metric)
{
  int i;
  double val;
  
  switch(metric)
    {
    case OMPP_CUBE_MET_OVHD_SYNC:
    case OMPP_CUBE_MET_OVHD_LIMPAR:
    case OMPP_CUBE_MET_OVHD_IMBAL:
    case OMPP_CUBE_MET_OVHD_MGMT:
      for( i=0; i<mstat->nexect && i<ompp_max_threads; i++ )
	{
	  cube_set_sev(cube, ompp_cube_mets[metric], 
		       cnode, ompp_cube_threads[i], mstat->exect[i]);
	  cube_set_sev(cube, ompp_cube_mets[OMPP_CUBE_MET_OVHDS], 
		       cnode, ompp_cube_threads[i], mstat->exect[i]);
	  cube_set_sev(cube, ompp_cube_mets[OMPP_CUBE_MET_TIME], 
		       cnode, ompp_cube_threads[i], mstat->exect[i]);
	}

      break;

    case OMPP_CUBE_MET_WORK:     
      for( i=0; i<mstat->nexect && i<ompp_max_threads; i++ )
	{
	  cube_set_sev(cube, ompp_cube_mets[OMPP_CUBE_MET_WORK], 
		       cnode, ompp_cube_threads[i], mstat->exect[i]);
	  
	  cube_set_sev(cube, ompp_cube_mets[OMPP_CUBE_MET_TIME], 
		       cnode, ompp_cube_threads[i], mstat->exect[i]);
	}
      
      break;

#ifdef OMPP_MPI
    case OMPP_CUBE_MET_MPITIME:
      fprintf(stderr, "MPITIME %f\n", mstat->time[i]);
      for( i=0; i<mstat->nctr && i<ompp_max_threads; i++ )
	cube_set_sev(cube,  ompp_cube_mets[OMPP_CUBE_MET_TIME],
		     cnode, ompp_cube_threads[i], mstat->time[i]);

      for( i=0; i<mstat->nctr && i<ompp_max_threads; i++ )
	cube_set_sev(cube, ompp_cube_mets[OMPP_CUBE_MET_OVHDS], 
		     cnode, ompp_cube_threads[i], mstat->time[i]);

      for( i=0; i<mstat->nctr && i<ompp_max_threads; i++ )
	cube_set_sev(cube, ompp_cube_mets[metric], 
		     cnode, ompp_cube_threads[i], mstat->time[i]);
      break;

    case OMPP_CUBE_MET_MPIDATAIN:
      fprintf(stderr, "MPIDATAIN %d\n", mstat->bin[i]);
      for( i=0; i<mstat->nctr && i<ompp_max_threads; i++ )
	cube_set_sev(cube,  ompp_cube_mets[OMPP_CUBE_MET_MPIDATAVOL],
		     cnode, ompp_cube_threads[i], mstat->bin[i]);
      for( i=0; i<mstat->nctr && i<ompp_max_threads; i++ )
	cube_set_sev(cube, ompp_cube_mets[metric], 
		     cnode, ompp_cube_threads[i], mstat->bin[i]);
      break;

    case OMPP_CUBE_MET_MPIDATAOUT:
      fprintf(stderr, "MPIDATAOUT\n");
      for( i=0; i<mstat->nctr && i<ompp_max_threads; i++ )
	cube_set_sev(cube,  ompp_cube_mets[OMPP_CUBE_MET_MPIDATAVOL],
		     cnode, ompp_cube_threads[i], mstat->bout[i]);
      for( i=0; i<mstat->nctr && i<ompp_max_threads; i++ )
	cube_set_sev(cube, ompp_cube_mets[metric], 
		     cnode, ompp_cube_threads[i], mstat->bout[i]);
       break;

    case OMPP_CUBE_MET_MPINSEND:
      fprintf(stderr, "MPISEND\n");
      for( i=0; i<mstat->nctr && i<ompp_max_threads; i++ )
	cube_set_sev(cube,  ompp_cube_mets[OMPP_CUBE_MET_MPINCALLS],
		     cnode, ompp_cube_threads[i], mstat->nsend[i]);
      for( i=0; i<mstat->nctr && i<ompp_max_threads; i++ )
	cube_set_sev(cube, ompp_cube_mets[metric], 
		     cnode, ompp_cube_threads[i], mstat->nsend[i]);
       break;

    case OMPP_CUBE_MET_MPINRECV:
      fprintf(stderr, "MPIRECV\n");
      for( i=0; i<mstat->nctr && i<ompp_max_threads; i++ )
	cube_set_sev(cube,  ompp_cube_mets[OMPP_CUBE_MET_MPINCALLS],
		     cnode, ompp_cube_threads[i], mstat->nrecv[i]);
      for( i=0; i<mstat->nctr && i<ompp_max_threads; i++ )
	cube_set_sev(cube, ompp_cube_mets[metric], 
		     cnode, ompp_cube_threads[i], mstat->nrecv[i]);
       break;

    case OMPP_CUBE_MET_MPINCOLL:
      fprintf(stderr, "MPINCOLL %d\n", mstat->ncoll[i]);
      for( i=0; i<mstat->nctr && i<ompp_max_threads; i++ )
	cube_set_sev(cube,  ompp_cube_mets[OMPP_CUBE_MET_MPINCALLS],
		     cnode, ompp_cube_threads[i], mstat->ncoll[i]);
      for( i=0; i<mstat->nctr && i<ompp_max_threads; i++ )
	cube_set_sev(cube, ompp_cube_mets[metric], 
		     cnode, ompp_cube_threads[i], mstat->ncoll[i]);
       break;
#endif 
       
    }

}


