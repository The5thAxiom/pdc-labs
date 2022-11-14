/*
 * ompp_eval.c  */
/* @OMPP_HEADER@ */ 


#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "ompp.h"

#ifdef OMPP_USE_PAPI

inline int istokenchar(char c)
{
  return isupper(c) || isdigit(c) || c=='_';
}


/*
 * s is the evaluator expression string
 * ctrnames holds the names of the counters, free counters
 *
 */
void ompp_eval_find_ctrs( char *s, char* ctrnames[] )
{
  int i;
  int ctrnum;
  size_t len = strlen(s);
  size_t start, end;
  char buf[100];

  start=0; end=0;

  while( start<len )
    {
      while( start<len && !(istokenchar(s[start])) )
	start++;
      
      end=start;

      i=0;
      while( end<len && (istokenchar(s[end])) ) 
	{
	  buf[i]=s[end];
	  end++; i++;
	}
      buf[i]=0;
      
      start=end; 
      start++;

      if( !isupper(buf[0]) )
	continue;

      //fprintf( stderr, " *** %s ***\n", buf);

      if( !strcmp(buf, "EXECT") )
	  continue;

      if( !strcmp(buf, "EXECC") )
	  continue;
	  
      ctrnum=-1;
      for( i=0; i<OMPP_PAPI_MAX_CTRS; i++ )
	{
	  if( ctrnames[i] && !strcmp(ctrnames[i], buf) )
	    {
	      ctrnum=i;
	      //fprintf(stderr, " ** ctr %s already programmed \n",buf);
	    }
	}

      if( ctrnum>=0 )
	continue;

      for( i=0; i<OMPP_PAPI_MAX_CTRS; i++ )
	{
	  if( !ctrnames[i] ) // this is a free slot
	    {
	      ctrnames[i]=strdup(buf);
	      //fprintf(stderr, " ** ctr %s programmed \n",buf);
	      break;
	    }
	}
    }
}

#endif

