
#include <stdio.h>

#include "color.h"

unsigned int coltab[1024]; /* color gradient table */


void coltab_init()
{
  int i, j;

  j=1023;
  for( i=0; i<256; i++ )
    {
      COL_SETRGB( coltab[j], 255, i, 0 );
      j--;
    }
  for( i=0; i<256; i++ )
    {
      COL_SETRGB( coltab[j], 255-i, 255, 0 );
      j--;
    }
  for( i=0; i<256; i++ )
    {
      COL_SETRGB( coltab[j], 0, 255, i );
      j--;
    }
  for( i=0; i<256; i++ )
    {
      COL_SETRGB( coltab[j], 0, 255-i, 255 );
      j--;
    }
}


int brightness(int color)
{
  int r, g, b;

  r = COL_GETR(color);
  g = COL_GETG(color);
  b = COL_GETB(color);
  
  return (r*299+g*587+b*114)/1000;
}


/*
int main(int argc, char* argv[])
{
  int i;
  coltab_init();

  for( i=0; i<1024; i++ )
    {
      fprintf(stdout, "%d %d %d %d brightness:%d\n", 
	      i, COL_GETR(coltab[i]), 
	      COL_GETG(coltab[i]), COL_GETB(coltab[i]),
	      brightness(coltab[i]));

      
    }
}
*/
