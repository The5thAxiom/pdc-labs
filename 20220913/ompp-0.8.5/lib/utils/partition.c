/* 
 * partition.c */
/* @OMPP_HEADER@ */

#include <stdio.h>

/* 
   IN: 
     - array 'value' of integers, size n, 
     - array 'partition' to hold partition assignment, size n 
   OUT: 
     - returns number of partitions discovered (P), 
     - partition assignment for each value in the
       range [0..P-1] in the array 'partition'
*/
int find_partitions( int *value, 
		     int *partition,
		     int n )
{
  int i, j;
  int npart;

  partition[0] = 0; 
  npart=1;
  for( j=1; j<n; j++ )
    {
      /* find partition for value[j] */
      for( i=0; i<j; i++ )
	{
	  if( value[i]==value[j] )
	    {
	      /* found matching previous partition */
	      partition[j]=partition[i];
	      break;
	    }
	}
      /* no matching previous partition */
      if( i==j )
	partition[j]=npart++;
    }

  return npart;
}



/* 
   IN: 
     - string 'str'
     - array 'partition' with integer partitions of size n
     - integer part to select a partition to be printed 
   OUT: 
     - formats a comma separated list of consecutive segments
       for the given partition in str
       Example: 0,3-5,7
     - returns number of consecutive segments in the partition 
       formatting (3 in the above example)
     - The list can be empty or a comma separated list of 
       segements
     - Single element segments are given without hyphen (7)
     - Multi element segments are given in hyphen notation (3-5)
*/
int format_partition( char *str, 
		      int *partition, int part, int n )
{
  int offs=0;
  int start, end;
  int nseg=0;

  start=end=-1;
  while( start<n )
    {
      start=end+1;
      while( start<n )
	{
	  if( partition[start]!=part )
	    start++;
	  else 
	    break;
	}
      
      end=start+1;
      while( end<n )
	{
	  if( partition[end]==part )
	    end++;
	  else 
	    break;
	}

      if( start<n && end>=(start+1) )
	{
	  if( nseg>0 )
	    offs+=sprintf(str+offs, ",");

	  if( end-1==start )
	    offs+=sprintf(str+offs, "%d", start);
	  else
	    offs+=sprintf(str+offs, "%d-%d", start, end-1);
	  nseg++;
	}
    }

  /* terminate string */
  str[offs]=0;
  return nseg;
}


/* 
   IN: 
     - array 'partition' with integer partitions of size n
     - integer 'part' to select which partition to a repre-
       sentative is to be found for
   OUT: 
     - returns an index in [0:n-1] for a representative for 
       the partition 'part', or -1 if such an representative 
       can not be found
*/
int get_representative( int *partition, int part, int n )
{
  int i;

  for( i=0; i<n; i++ )
    {
      if( partition[i]==part )
	return i;
    }
  return -1;
}
