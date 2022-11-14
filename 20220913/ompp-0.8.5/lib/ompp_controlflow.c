
#include <float.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "ompp.h"
#include "ompp_region.h"
#include "ompp_rstack.h"
#include "ompp_regstat.h"
#include "utils/partition.h"
#include "utils/color.h"

/* uncomment one color scheme */
//#define CONTROLFLOW_COLOR
#define CONTROLFLOW_GREY 
//#define CONTROLFLOW_WHITE
//#define CONTROLFLOW_YELLOW

/* uncomment to show timing data */
#define CONTROLFLOW_TIME

#ifdef OMPP_CONTROLFLOW


#define FLOWINFO_ROOT      "[ INIT ] { shape: point; pointstyle: circle; }"
#define FLOWINFO_PARENT    ".. %s ..> { flow: south; }"
#define FLOWINFO_SIBLING   "-- %s -->"


#define FLOWINFO_EXECC                1
#define FLOWINFO_REGNUM           (1<<1)
#define FLOWINFO_RSTATNUM         (1<<2)
#define FLOWINFO_RTYPE            (1<<3)
#define FLOWINFO_FILELINE         (1<<4)
#define FLOWINFO_CHILDIND         (1<<5)
#define FLOWINFO_PARENTIND        (1<<6)
#define FLOWINFO_CPUTIME          (1<<7)
#define FLOWINFO_CPUTIMEX         (1<<8)
#define FLOWINFO_COLOR_GRAD       (1<<9) /* blue to red gradient */
#define FLOWINFO_COLOR_GREY      (1<<10) /* greyscale gradient */
#define FLOWINFO_COLOR_WHITE     (1<<11)
#define FLOWINFO_COLOR_YELLOW    (1<<12)
#define FLOWINFO_CHILDLINK       (1<<13)
#define FLOWINFO_SELFLINK        (1<<14)
#define FLOWINFO_PARENTLINK      (1<<15)


/* default is white */
#define FLOWINFO_NODECOLOR  FLOWINFO_COLOR_WHITE
#define FLOWINFO_SCALECOLOR FLOWINFO_COLOR_WHITE

#ifdef CONTROLFLOW_COLOR
#undef FLOWINFO_NODECOLOR
#undef FLOWINFO_SCALECOLOR
#define FLOWINFO_NODECOLOR  FLOWINFO_COLOR_GRAD
#define FLOWINFO_SCALECOLOR FLOWINFO_COLOR_GRAD
#endif

#ifdef CONTROLFLOW_GREY
#undef FLOWINFO_NODECOLOR
#undef FLOWINFO_SCALECOLOR
#define FLOWINFO_NODECOLOR  FLOWINFO_COLOR_GREY
#define FLOWINFO_SCALECOLOR FLOWINFO_COLOR_GREY
#endif

#ifdef CONTROLFLOW_YELLOW
#undef FLOWINFO_NODECOLOR
#undef FLOWINFO_SCALECOLOR
#define FLOWINFO_NODECOLOR  FLOWINFO_COLOR_YELLOW
#define FLOWINFO_SCALECOLOR FLOWINFO_COLOR_YELLOW
#endif


int getcputime_color( double val );
int getcputime_index( double val );

void rstack_get_minmax( rstack_t *st, 
			double *min, double *max );


/* FIXME */
int get_execc(regstat_t *rstat)
{
  int res, i;

  switch( rstat->reg->type )
    {
    case OMPP_TASKEXEC:
      res = 0;
      for( i=0; i<rstat->main->nexect; i++ )
	{
	  res += rstat->main->execc[i];
	}
      break;

    default:
      res = rstat->main->execc[0];
      break;
    }

  return res;
}


void format_node( char *basename, char *str, 
		  rnode_t *node, 
		  ompp_region_t *reg, int format )
{
  char buf[80];
  double cput;
  unsigned col, r,g,b;

  if( !node || !reg )
    {
      strcat(str, FLOWINFO_ROOT);
      return;
    }

  strcat(str, "[ ");
  if( format&FLOWINFO_EXECC )
    {
      sprintf(buf, "%dx ", get_execc(node->rstat));
      strcat(str, buf);
    }

  if( format&FLOWINFO_REGNUM )
    {
      sprintf(buf, "R%05u", reg->number);
      strcat(str, buf);
    }

  if( format&FLOWINFO_RSTATNUM )
    {
      sprintf(buf, ".%u", node->rstatnum);
      strcat(str, buf);
    }

  if( format&FLOWINFO_CHILDIND )
    {
      if( node->child )
	{
	  strcat(str, " (+)");
	}
    }

  if( format&FLOWINFO_PARENTIND )
    {
      if( node->parent )
	{
	  strcat(str, " (^)");
	}
    }


  if( format&FLOWINFO_RTYPE )
    {
      //sprintf(buf, " %s", ompp_region_full_name[reg->type]);
      sprintf(buf, "\\n%s", ompp_region_full_name[reg->type]);
      strcat(str, buf);      
    }

  if( format&FLOWINFO_FILELINE )
    {
      sprintf(buf, "\\n%s", reg->location);
      strcat(str, buf);
    }

  if( format&FLOWINFO_CPUTIMEX )
    {
      cput = regstat_getcputime_x(node->rstat);
      sprintf(buf, "\\n%5.2f", cput);
      strcat(str, buf);      
    }

  if( format&FLOWINFO_CPUTIME )
    {
      cput = regstat_getcputime(node->rstat);
      sprintf(buf, "\\n%5.2f", cput);
      strcat(str, buf);      
    }

  strcat(str, " ]");

  /* handle color */
  if( format&FLOWINFO_COLOR_GRAD ) {
    if( format&FLOWINFO_CPUTIMEX )
      cput = regstat_getcputime_x(node->rstat);
    else
      cput = regstat_getcputime(node->rstat);
    col = getcputime_color(cput);
  }
  else if( format&FLOWINFO_COLOR_GREY ) {
    int idx;
    if( format&FLOWINFO_CPUTIMEX )
      cput = regstat_getcputime_x(node->rstat);
    else
      cput = regstat_getcputime(node->rstat);
    idx=getcputime_index(cput);
    idx = 255-(int)((255.0*(double)(idx))/((double)(COLTAB_SIZE)));
    COL_SETRGB(col, idx,idx,idx);
  }
  else if( format&FLOWINFO_COLOR_WHITE ) {
    COL_SETRGB(col, 255,255,255);
  }
  else if( format&FLOWINFO_COLOR_YELLOW ) {
    COL_SETRGB(col, 255,255,0);
  }
  else {
    COL_SETRGB(col, 255,255,255);
  }
  
  r = COL_GETR(col);
  g = COL_GETG(col);
  b = COL_GETB(col);
  
  if( brightness(col)<100 )
    sprintf(buf, " { color:white; fill: rgb(%d,%d,%d); shape: rounded; } ", r, g, b);
  else
    sprintf(buf, " {fill: rgb(%d,%d,%d); shape: rounded; } ", r, g, b);
  
  strcat(str, buf);
  
  if( format&FLOWINFO_CHILDLINK )
    {
      if( node->child && (node->child)->reg )
	{
	  ompp_region_t *child = node->child->reg;
	  
	  sprintf(buf, "{linkbase: ; link: %s%sflowinfo.R%05u.%d.svg; } ", 
		  basename, (*basename)!=0?".":"", 
		  child->number,
		  node->child->rstatnum);



	  strcat(str, buf);
	}
    }

  if( format&FLOWINFO_SELFLINK )
    {
      if( node->child )
	{
	  sprintf(buf, "{linkbase: ; link: %s%sflowinfo.R%05u.%d.svg; } ",
		  basename, (*basename)!=0?".":"",
		  node->reg->number,
		  node->rstatnum);
	  
	  strcat(str, buf);
	}
    }

  if( format&FLOWINFO_PARENTLINK )
    {
      if( node->parent && (node->parent)->reg )
	{
	  sprintf(buf, "{linkbase: ; link: %s%sflowinfo.R%05u.%d.svg; } ", 
		  basename, (*basename)!=0?".":"", 
		  node->parent->reg->number,
		  node->parent->rstatnum);

	  strcat(str, buf);
	}
      else
	{
	  sprintf(buf, "{linkbase: ; link: %s%sflowinfo.ROOT.svg; } ", 
		 basename, (*basename)!=0?".":"" );

	  strcat(str, buf);
	}
      
    }

}


void flowinfo_print_scale( char *basename, int nlevels )
{
  FILE *file;
  int i, i1, i2;
  double min, max, v1, v2;
  unsigned c1, c2;
  char buf[200], fname[200];
  unsigned r,g,b;
  char *textcolor="black";
  
  rstack_get_minmax( &ompp_rstack, &min, &max );

  sprintf(fname, "%s%sflowinfo.SCALE.txt", 
	  basename, (*basename)!=0?".":"");
  file = fopen(fname, "w");

  for( i=0; i<nlevels+1; i++ )
    {
      v1 = min+(max-min)/((double)(nlevels))*((double)(i));
      //v2 = min+(max-min)/((double)(nlevels))*((double)(i+1));
      
      c1 = getcputime_color(v1);
      //c2 = getcputime_color(v2);

      i1 = getcputime_index(v1);
      //i2 = getcputime_index(v2);

      if( FLOWINFO_COLOR_GREY&FLOWINFO_SCALECOLOR ) {
	r=g=b=255-(int)((255.0*(double)(i1))/((double)(COLTAB_SIZE)));

	if( r<120 )
	  textcolor="white";
      }
      
      if( FLOWINFO_COLOR_GRAD&FLOWINFO_SCALECOLOR ) {
	r=COL_GETR(c1);
	g=COL_GETG(c1);
	b=COL_GETB(c1);
	
	if( brightness(c1)<100 )
	  textcolor="white";
      }

      if( FLOWINFO_COLOR_WHITE&FLOWINFO_SCALECOLOR ) {
	r=255;
	g=255;
	b=255;
      }

      if( FLOWINFO_COLOR_YELLOW&FLOWINFO_SCALECOLOR ) {
	r=255;
	g=255;
	b=0;
      }

      sprintf(buf, " [ %02d: %5.2f ] { color:%s; fill: rgb(%d,%d,%d); }\n",
	      i, v1, textcolor, r, g, b );
      
      fprintf(file, buf);
    }
  
  fclose(file);
}
       	  

/*
 * dump the control flow information at this node of 
 * the region stack
 */
void flowinfo_dump_node( FILE *f, rnode_t *node )
{
  int i, j;

  fprintf(f, "--- flowinfo dump for node %p reg=%s\n",
	  node, node->reg->location );

  for( i=0; i<OMPP_MAX_REGIONS; i++ )
    {
      for( j=0; j<ompp_max_threads; j++ )
	{
	  fprintf(f, "%d ", node->predcount[j][i] );
	}
      fprintf(f, "\n");
    }
}



/*
 * print the control flow information in a format
 * consumable by Graph::Easy
 *
 * node is the control flow node to be printed, 
 * root points to the root node of graph to be printed,
 * this will be the level-root for level-by-level printing 
 * and the overall root for the overall printing
 */
void flowinfo_print_node( int levels, char *basename, 
			  FILE *f, rnode_t *node )
{
  int i, j;
  int sum;
  rnode_t       *fromnode;
  ompp_region_t *fromreg;
  int nparts, *parts, *vals;
  int nodefmt=0;

  nodefmt =  FLOWINFO_EXECC |
    FLOWINFO_REGNUM |
    FLOWINFO_RSTATNUM |
    FLOWINFO_RTYPE | 
    FLOWINFO_FILELINE |
    FLOWINFO_NODECOLOR;

  /* used for partition assignment */
  parts = (int*) malloc(sizeof(int)*ompp_max_threads);
  vals  = (int*) malloc(sizeof(int)*ompp_max_threads);

  for( i=0; i<OMPP_MAX_REGIONS; i++ )
    {
      /* find out if this region is on the control flow 
	 of the current node, if not, skip to the next 
	 region */
      sum=0;
      for( j=0; j<ompp_max_threads; j++ )
	{
	  vals[j] = node->predcount[j][i];
	  sum+=vals[j];
	}
      if( sum==0 )
	continue;


      /* find the 'sending' side (node) of the link */
      fromreg = ompp_get_region_by_number(i);
      fromnode=0;
      if( node->parent )
	{
	  if( node->parent->reg==fromreg )
	    {
	      fromnode = node->parent;
	    }
	  else
	    {
	      fromnode=node->parent->child;
	      while( fromnode )
		{
		  if( fromnode->reg==fromreg )
		    break;
		  fromnode=fromnode->next;
		}
	      
	    }
	}

      /* find partions and format them */
      nparts=find_partitions( vals, parts, ompp_max_threads );
      for( j=0; j<nparts; j++ )
	{
	  char from[256], line[256], to[256];
	  char buf[256];
	  int rep, ntrans;
	  
	  /* find a representative index of this partition and
	     find out how many transitions it represents,
	     don't print zeroes */
	  rep=get_representative(parts, j, ompp_max_threads);
	  ntrans=vals[rep];
	  if( ntrans==0 )
	    continue;

	  format_partition( buf, parts, j, ompp_max_threads );
	  sprintf(buf+strlen(buf), "/%d", ntrans);
  
	  *from=0; *to=0;
	  /* it is a parent or sibling link? */
	  if( node->parent && (node->parent->reg)==fromreg )
	    {
	      sprintf(line, FLOWINFO_PARENT, buf );

	      format_node( basename, from, fromnode, fromreg, 
			   nodefmt 
#ifdef CONTROLFLOW_TIME 
			   | FLOWINFO_CPUTIMEX 
#endif
			   | (levels?FLOWINFO_PARENTIND:0) 
			   | FLOWINFO_PARENTLINK );
	      
	      format_node( basename, to, node, node->reg, 
			   nodefmt 
#ifdef CONTROLFLOW_TIME 
			   | (levels?FLOWINFO_CPUTIME:FLOWINFO_CPUTIMEX)
#endif
			   | (levels?FLOWINFO_CHILDIND:0)
			   | FLOWINFO_SELFLINK );
	    }
	  else
	    {
	      sprintf(line, FLOWINFO_SIBLING, buf );

	      format_node( basename, from, fromnode, fromreg, 
			   nodefmt
#ifdef CONTROLFLOW_TIME
			   | (levels?FLOWINFO_CPUTIME:FLOWINFO_CPUTIMEX)
#endif
			   | (levels?FLOWINFO_CHILDIND:0)
			   | FLOWINFO_SELFLINK );
	      format_node( basename, to, node, node->reg, 
			   nodefmt
#ifdef CONTROLFLOW_TIME
			   | (levels?FLOWINFO_CPUTIME:FLOWINFO_CPUTIMEX)
#endif
			   | (levels?FLOWINFO_CHILDIND:0)
			   | FLOWINFO_SELFLINK );
	    }
	  
	  fprintf(f, "%s %s %s\n", from, line, to );
	}
    }


  free(parts);
  free(vals);
}



/*
 * print the control flow information for the
 * whole application
 */
void flowinfo_print_full( char *basename, rstack_t *st )
{
  FILE *file;
  char fname[200];
  rnode_t *node, *start;

  sprintf(fname, "%s%sflowinfo.txt", 
	  basename, (*basename)!=0?".":"");
  file = fopen(fname, "w");
  
  /* start at the root */
  start=&(st->root);
  while(start)
    {
      /* sweep over nodes at this level */
      node = start;
      while(node) 
	{
	  if( !(node->reg) )
	    {
	      node=node->next;
	      break;
	    }
	  
	  /* do something with the node */
	  flowinfo_print_node(0, basename, file, node);
	  
	  node=node->next;
	}

      /* we've done with the sweep, start is still where 
	 we started, now try to go to the child level if 
	 this is possible, otherwise go one level up  */
      if( start->child )
	{
	  start=start->child;
	}
      else
	{
	  /* backtrack */
	  while( start )
	    {
	      if( (start->next) ) 
		{
		  if( start->next->child )
		    {
		      start=start->next->child;
		      break;
		    }
		  else
		    {
		      start=start->next;
		    }
		}
	      else
		{
		  start=start->parent;
		}
	    }
	}
    }

  fclose(file);
}


/*
 * print the control flow information for each 
 * level of the control flow graph separately
 */
void flowinfo_print_levels( char* basename, rstack_t *st )
{
  char fname[200];
  FILE *file;
  rnode_t *node, *start;

  /* start at the root */
  start=(&(st->root))->child;
  while(start)
    {
      if( start->parent && start->parent->reg )
	{
	  sprintf(fname, "%s%sflowinfo.R%05u.%d.txt", 
		  basename, (*basename)!=0?".":"", 
		  start->parent->reg->number, 
		  start->parent->rstatnum);
	}
      else
	{
	  sprintf(fname, "%s%sflowinfo.%s.txt", 
		  basename, (*basename)!=0?".":"",
		  "ROOT");
	}
      file = fopen(fname, "w");

      /* sweep over nodes at this level */
      node = start;
      while(node) 
	{
	  if( !(node->reg) )
	    {
	      node=node->next;
	      break;
	    }
	  
	  /* do something with the node */
	  flowinfo_print_node(1, basename, file, node );
	  
	  node=node->next;
	}

      fclose(file);

      /* we've done with the sweep, start is still where 
	 we started, now try to go to the child level if 
	 this is possible, otherwise go one level up  */
      if( start->child )
	{
	  start=start->child;
	}
      else
	{
	  /* backtrack */
	  while( start )
	    {
	      if( (start->next) ) 
		{
		  if( start->next->child )
		    {
		      start=start->next->child;
		      break;
		    }
		  else
		    {
		      start=start->next;
		    }
		}
	      else
		{
		  start=start->parent;
		}
	    }
	}
    }
}





void rstack_get_minmax( rstack_t *st, 
			double *min, double *max )
{
  rnode_t *node, *start;
  double val;

  *min = DBL_MAX;
  *max = 0;

  /* start at the root */
  start=&(st->root);
  while(start)
    {
      /* sweep over nodes at this level */
      node = start;
      while(node) 
	{
	  if( !(node->reg) )
	    {
	      node=node->next;
	      break;
	    }
	  
	  /* do something with the node */
	  val = regstat_getcputime(node->rstat);
	  if( val<*min )
	    *min = val;
	  if( val>*max )
	    *max = val;

	  node=node->next;
	}

      /* we've done with the sweep, start is still where 
	 we started, now try to go to the child level if 
	 this is possible, otherwise go one level up  */
      if( start->child )
	{
	  start=start->child;
	}
      else
	{
	  /* backtrack */
	  while( start )
	    {
	      if( (start->next) ) 
		{
		  if( start->next->child )
		    {
		      start=start->next->child;
		      break;
		    }
		  else
		    {
		      start=start->next;
		    }
		}
	      else
		{
		  start=start->parent;
		}
	    }
	}
    }
}


int getcputime_index( double val )
{
  int idx;
  static int haveminmax=0;
  static double min;
  static double max; 

  if( !haveminmax ) 
    {
      coltab_init();
      rstack_get_minmax( &ompp_rstack, &min, &max );
      haveminmax=1;
    } 

  idx=(int)(((double)(COLTAB_SIZE-1))*(val-min)/(max-min));

  if( val<min )
    idx = 0;
  if( val>max )
    idx = COLTAB_SIZE-1; 
  
  return idx;
}

int getcputime_color( double val )
{
  int idx = getcputime_index(val);
  return coltab[idx];
}

#endif 
