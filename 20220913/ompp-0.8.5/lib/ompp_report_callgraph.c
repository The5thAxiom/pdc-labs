/* 
 * ompp_report_callgraph.c 
 *
 * Routines for writing ompP's callgraph and callgraph 
 * profiles. */
/* @OMPP_HEADER@ */

#include <stdio.h>
#include <string.h>

#include "ompp_rstack.h"
#include "ompp_regstat.h"
#include "ompp_region.h"
#include "ompp_report_callgraph.h"


/* -- callgraph  -- */

void print_callgraph( FILE *f, rstack_t *st )
{
  int levl;
  rnode_t *node;

  // print the header
  print_callgraph_header(f);

  // we need this as the termination criterion later
  assert( (st->root).parent==0 );

  levl=0;
  node=&(st->root);

  while(node)
    {
      // do the work on the node
      print_callgraph_node( f, node, levl );

      // advance to next node (depth-first)
      if( node->child ) 
	{
	  levl++;
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
	      levl--;
	     
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

  // print the footer
  print_callgraph_footer(f);  
}



void print_callgraph_header( FILE *f )
{
  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "----------------------------------------------------------------------\n");
      fprintf(f, "----     ompP Callgraph     ------------------------------------------\n");
      fprintf(f, "----------------------------------------------------------------------\n");
      fprintf(f, "\n");
      break;
      
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##BEG ompp callgraph\n");
      break;
    }
}
	

void print_callgraph_footer( FILE *f )
{	
  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf( f, "\n" );
      break;

    case OMPP_OUTFORMAT_CSV:
      fprintf( f, "##END ompp callgraph\n" );
      fprintf( f, "\n" );
      break;
    }
}



void print_callgraph_node( FILE *f, rnode_t *node, int levl )
{
  int i;
  rnode_t *tmp;
  double inclt; // inclusive cpu time, summed over all threads
  double exclt; // exclusive cpu time, summed over all threads
  double appt; // cpu time for the whole application
  const char *grph[OMPP_MAX_CALLGRAPH_DEPTH];
  double p1, p2; // percentages

  appt = ompp_duration;

  assert(node);

  // compute the exculsive stats for the node
  rstack_compute_xstats(node);

  if( !(node->parent) )
    {
      // we are at the root node!

      inclt=appt;
      exclt=appt;
      for( tmp=node->child; tmp; tmp=tmp->next )
	{
	  exclt-=regstat_getwtime( tmp->rstat );
	}
      
      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  fprintf(f, "  Inclusive  (%%)   Exclusive  (%%)\n");
	  fprintf(f, "%7.2f (100.0%%) ", inclt );
	  fprintf(f, "%7.2f (%5.2f%%) ", exclt, 100.0*(exclt/appt) );
	  fprintf(f, "          " );
	  fprintf(f, "[%s: %d threads]\n", 
		  ompp_executable_name, ompp_max_threads );
	  break;
	  
	case OMPP_OUTFORMAT_CSV:
	  fprintf(f, "Inclusive%c(%%)%cExclusive%c(%%)\n", 
		  ompp_sep, ompp_sep, ompp_sep );
	  fprintf(f, "%f%c100.0%c", inclt, ompp_sep, ompp_sep );
	  fprintf(f, "%f%c%f%c", exclt, ompp_sep, 
		  100.0*(exclt/appt), ompp_sep );
	  fprintf(f, "0%c[%s: %d threads]\n", 
		  ompp_sep, ompp_executable_name, ompp_max_threads );
	  break;
	}  

      return;
    }
  else
    {
      assert( node->reg );
      assert( levl<OMPP_MAX_CALLGRAPH_DEPTH );
      assert( levl>0 );  // only root has levl==0

      if( node->next )
	grph[levl-1]="|-";
      else
	grph[levl-1]="+-";

      tmp=node->parent;
      assert(tmp);

      for( i=levl-2; i>=0; i-- )
	{
	  if( tmp->next )
	    grph[i]="| ";
	  else
	    grph[i]="  ";
	  tmp=tmp->parent;
	}

      inclt=regstat_getwtime( node->rstat );
      exclt=regstat_getwtime_x( node->rstat );

      switch( ompp_outformat )
	{
	  const char *fmt;
	  
	case OMPP_OUTFORMAT_PLAIN:      
	  p1 = 100.0*(inclt/appt);
	  p2 = 100.0*(exclt/appt);

	  fmt="%7.2f (%5.2f%%) %7.2f (%5.2f%%)";

	  if( p1>99.9 && p2>99.9 )
	    fmt = "%7.2f (%5.1f%%) %7.2f (%5.1f%%)";
	  if( p1>99.9 && p2<0.1 )
	    fmt = "%7.2f (%5.1f%%) %7.2f (%5.3f%%)";
	  if( p1<0.1 && p2>99.9 )
	    fmt = "%7.2f (%5.3f%%) %7.2f (%5.1f%%)";
	  if( p1<0.1 && p2<0.1 )
	    fmt = "%7.2f (%5.3f%%) %7.2f (%5.3f%%)";

	  fprintf(f, fmt, 
		  inclt, p1, exclt, p2);
	  
	  fprintf(f, " %8s ", ompp_region_name[node->reg->type] );
	  
	  for( i=0; i<levl; i++ )
	    {
	      fprintf(f, OMPP_CALLGRAPH_STRFMT, grph[i]);
	    }
	  fprintf(f, "R%05u %s\n",
		  node->reg->number,
		  node->reg->location);
	  break;
	  
	case OMPP_OUTFORMAT_CSV:

	  fprintf(f,"%f%c%f%c%f%c%f%c", 
		  inclt, ompp_sep,
		  100.0*(inclt/appt), ompp_sep,
		  exclt, ompp_sep,
		  100.0*(exclt/appt), ompp_sep );

	  fprintf( f, "%d%c", levl, ompp_sep );

	  fprintf(f, "R%05u%c%s%c%s\n",
		  node->reg->number, ompp_sep, 
		  ompp_region_full_name[node->reg->type], ompp_sep, 
		  node->reg->csvloc );

	  break;
	}
    }
}
			     

/* -- callgraph profiles -- */

void print_callgraph_profiles( FILE *f, rstack_t *st )
{
  int levl;
  rnode_t *node;

  // header
  print_callgraph_profiles_header(f);

  // we need this as the termination criterion later
  assert( (st->root).parent==0 );

  levl=0;
  node=&(st->root);
  while(node)
    {
      // do something with the node!

      // compute exclusive stats
      rstack_compute_xstats(node);
      print_callgraph_profiles_node( f, node, levl );

      // advance to next node (depth-first)
      if( node->child ) 
	{
	  levl++;
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
	      levl--;
	     
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

  // footer
  print_callgraph_profiles_footer(f);
}


void print_callgraph_profiles_header( FILE *f )
{
  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "----------------------------------------------------------------------\n");
      fprintf(f, "----     ompP Callgraph Region Profiles (incl./excl. data)     -------\n");
      fprintf(f, "----------------------------------------------------------------------\n");
      fprintf(f, "\n");
      break;
      
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##BEG callgraph region profiles\n");
      break;
    }
}


void print_callgraph_profiles_footer( FILE *f )
{
  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "\n" );
      break;
      
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "##END callgraph region profiles\n");
      fprintf(f, "\n" );
      break;
    }
}


void print_callgraph_profiles_node( FILE *f, 
				    rnode_t *node, int levl )
{
  int i;
  rnode_t *tmp;
  rnode_t *cpath[OMPP_MAX_CALLGRAPH_DEPTH];

  assert(node);

  if( !(node->parent) ) 
    {
      // nothing to do at the root node!
      return;
    }

  assert(node->rstat);
  assert(node->reg);

  if( ompp_outformat==OMPP_OUTFORMAT_CSV )
    {
      fprintf(f, "##BEG R%05u callgraph profile\n", node->reg->number);
    }
  
  //regstat_print_header( node->rstat, f );

  // build callpath back to root node
  tmp=node;
  for( i=levl; i>=0; i-- )
    {
      assert(tmp);
      cpath[i]=tmp;
      tmp=tmp->parent;
    }
  
  switch( ompp_outformat )
    {
    case OMPP_OUTFORMAT_PLAIN:
      fprintf(f, "[%s%02d] %s\n", "*", 0, ompp_executable_name );
      break;
    case OMPP_OUTFORMAT_CSV:
      fprintf(f, "[%s%02d]%c%s\n", "*", 0, ompp_sep, ompp_executable_name );
      break;
    }
  
  for( i=0; i<=levl; i++ )
    {
      tmp=cpath[i];
      if( !tmp || !(tmp->reg) ) continue;

      switch( ompp_outformat )
	{
	case OMPP_OUTFORMAT_PLAIN:
	  fprintf(f, "[%s%02d] R%05u %s %s\n",
		  (tmp->child?"+":"="), i, tmp->reg->number,
		  tmp->reg->location,
		  ompp_region_full_name[tmp->reg->type] );
	  break;
	  
	case OMPP_OUTFORMAT_CSV:
	  fprintf(f, "[%s%02d]%cR%05u%c%s%c%s\n",
		  (tmp->child?"+":"="), i, ompp_sep,
		  tmp->reg->number, ompp_sep,
		  ompp_region_full_name[tmp->reg->type], ompp_sep, 
		  tmp->reg->csvloc );

	  break;
	}
    }

  regstat_print( f, node->rstat, 1 );

  if( ompp_outformat==OMPP_OUTFORMAT_CSV )
    {
      fprintf(f, "##END R%05u callgraph profile\n", node->reg->number);
    }
  
  fprintf(f, "\n");
}


