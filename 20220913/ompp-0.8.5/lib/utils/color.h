

#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#define COLTAB_SIZE 1024

#define COL_SETR(c, r) (c = (0xFF00FFFF&c)+(r<<16))
#define COL_SETG(c, g) (c = (0xFFFF00FF&c)+(g<<8))
#define COL_SETB(c, b) (c = (0xFFFFFF00&c)+b)

#define COL_SETRGB(c, r,g,b) (c = (0xFFFFFF00&c)+b + \
			      (0xFFFF00FF&c)+(g<<8) + \
			      (0xFF00FFFF&c)+(r<<16))

#define COL_GETR(c) ((c&0x00FF0000)>>16)
#define COL_GETG(c) ((c&0x0000FF00)>>8)
#define COL_GETB(c) ((c&0x000000FF))


void coltab_init();

extern unsigned int coltab[COLTAB_SIZE];
extern int brightness(int color);

#endif /* COLOR_H_INCLUDED */

