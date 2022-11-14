

#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "utils/list.h"
#include "ompp_rstack.h"
#include "ompp_region.h"
#include "ompp_report_xml.h"


typedef void(*nfunc_t)(FILE*, rnode_t*, int);
typedef void(*regfunc_t)(FILE *, ompp_region_t*);


void ompp_traverse_calltree(FILE*, rstack_t *st, nfunc_t func);
void ompp_traverse_regions(FILE*, dllist_t *rlist, regfunc_t func);

//void ompp_xml_write_regions(FILE *f, dllist_t *rlist);

void xml_compute_xstats(FILE *f, rnode_t *node, int level)
{
  rstack_compute_xstats(node);
}

void print_cnode(FILE *f, rnode_t *node, int level)
{
  rnode_t *p;
  int i, nchild;

  if( !node )
    return;

  nchild=0; p=node->child;
  while( p ) {
    nchild++;
    p=p->next;
  }
  

  if( !node->parent ) {
    fprintf(f, "<cnode id=\"%d\" regid=\"0\" level=\"0\" nchild=\"%d\">\n",
	    node->id, nchild);
    return;
  }
    
  for(i=0; i<level; i++ )
    fprintf(f, "  ");
  fprintf(f, "<cnode id=\"%d\" regid=\"%d\" level=\"%d\" nchild=\"%d\">", 
	  node->id, node->reg->number, level, nchild);

  
  if( !node->child ) {
    fprintf(f, "</cnode>\n");

    if( !node->next )
      {
	p=node->parent; level--;
	while(p)
	  {
	    if( !node->next )
	      {
		for(i=0; i<level; i++ )
		  fprintf(f, "  ");
		fprintf(f, "</cnode>\n");
	      }
	    node=node->parent;
	    p=node->parent; level--;
	  }
      }
  }
  else
    fprintf(f, "\n");
  
}

void print_region(FILE *f, ompp_region_t *reg)
{
  fprintf(f, "<region id=\"%d\" type=\"%s\" name=\"%s\" file=\"%s\" beg=\"%d\" end=\"%d\">",
	  reg->number,
	  ompp_region_name[reg->type],
	  reg->desc->sub_name,
	  reg->desc->file_name,
	  reg->desc->begin_first_line,
	  reg->desc->end_first_line);
  fprintf(f, "%s %s", 
	  ompp_region_full_name[reg->type], 
	  reg->location);
  fprintf(f, "</region>\n");
}

void print_rstat_perfdata(FILE *f, rnode_t* node, 
			  mstat_t *mstat, unsigned metric, const char *name )
{
  int i;
  static const char *mclass[] = { 
    "time", "visits", "work", "ovhds", 
    "sync", "limpar", "imbal", "mgmt"};
  
  switch( metric )
    {
    case  OMPP_XML_MET_TIME:
    case  OMPP_XML_MET_WORK:
    case  OMPP_XML_MET_OVHDS:
    case  OMPP_XML_MET_OVHD_SYNC:
    case  OMPP_XML_MET_OVHD_LIMPAR:
    case  OMPP_XML_MET_OVHD_IMBAL:
    case  OMPP_XML_MET_OVHD_MGMT:
      fprintf(f, "  <pd type=\"time\" class=\"%s\" name=\"%s\">", 
	      mclass[metric], name);
      for( i=0; i<mstat->nexect && i<ompp_max_threads; i++ )
	{
	  fprintf(f, "%f ", mstat->exect[i]);
	}
      break;
    case OMPP_XML_MET_VISITS:
      fprintf(f, "  <pd type=\"count\" class=\"%s\" name=\"%s\">", 
	      mclass[metric], name);
      for( i=0; i<mstat->nexect && i<ompp_max_threads; i++ )
	{
	  fprintf(f, "%d ", mstat->execc[i]);
	}
      break;
    }
  fprintf(f, "</pd>\n");
}



void print_cnode_rstats(FILE *f, rnode_t *node, int level)
{
  ompp_region_t *reg;
  regstat_t *rstat;
  
  reg=node->reg;
  rstat=node->rstat;
  
  if( !reg || !rstat )
    return;

  fprintf(f, "<rstat cnodeid=\"%d\">\n", node->id);
  switch( reg->type )
    {
    case OMPP_PARALLEL:
    case OMPP_PARALLEL_LOOP:
    case OMPP_PARALLEL_WORKSHARE:
    case OMPP_PARALLEL_SECTIONS:
      print_rstat_perfdata(f, node, rstat->enter,
			   OMPP_XML_MET_OVHD_MGMT, "enterT");
      print_rstat_perfdata(f, node, rstat->exit,
			   OMPP_XML_MET_OVHD_MGMT, "exitT");
      print_rstat_perfdata(f, node, rstat->ibarr,
			   OMPP_XML_MET_OVHD_IMBAL, "exitBarT");
      print_rstat_perfdata(f, node, rstat->body_x, 
			   OMPP_XML_MET_WORK, "bodyT");
      print_rstat_perfdata(f, node, rstat->body,
			   OMPP_XML_MET_VISITS, "bodyC");

      break;
    }
  fprintf(f, "</rstat>\n");
}


void ompp_report_xml( FILE* f, 
		      dllist_t *rlist,
		      rstack_t *rstack )
{
  int i;

  fprintf(f, "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n");

  fprintf(f, "<report>\n");
  ompp_traverse_regions(f, rlist, print_region);
  
  ompp_traverse_calltree(f, rstack, print_cnode);
  ompp_traverse_calltree(f, rstack, xml_compute_xstats);
  ompp_traverse_calltree(f, rstack, print_cnode_rstats);

  fprintf(f, "</report>\n");
  //  fprintf(f, "</xml>\n");
}
 

void ompp_traverse_calltree(FILE *f, rstack_t *st, nfunc_t func)
{
  int levl;
  rnode_t *node;

  /* we need this as the termination criterion later */
  assert( (st->root).parent==0 );

  levl=0;
  node=&(st->root);
  while(node)
    {
      /* do something with the node */
      (*func)(f, node, levl);
      
      /* advance to next node (depth-first) */
      if( node->child ) 
	{
	  levl++;
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



void ompp_traverse_regions(FILE *f, dllist_t *rlist, regfunc_t func)
{
  lelem_t *elem; 

  elem=rlist->head;
  while( elem )
    {
      func(f, (ompp_region_t*)elem->data);

      elem = elem->next;
    }

}
