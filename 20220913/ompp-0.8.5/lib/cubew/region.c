/****************************************************************************
**  SCALASCA    http://www.scalasca.org/                                   **
**  KOJAK       http://www.fz-juelich.de/jsc/kojak/                        **
*****************************************************************************
**  Copyright (c) 1998-2008                                                **
**  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
**                                                                         **
**  Copyright (c) 2003-2008                                                **
**  University of Tennessee, Innovative Computing Laboratory               **
**                                                                         **
**  See the file COPYRIGHT in the package base directory for details       **
****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "region.h"

typedef struct rarray {
  int size;
  int capacity;
  struct cube_cnode** data;
} rarray;


cube_region* cube_region_create(cube_region* thisp) {
  if (thisp == NULL) {
    thisp = (cube_region*) malloc(sizeof(cube_region));
    if (thisp == NULL) {
      printf("Malloc error\n");
      exit(1);
    }
  }
  cube_region_construct_cnode(thisp);
  return thisp;
}

void cube_region_init(cube_region* thisp, char* name, int begln, int endln, 
                      char* url, char* descr, char* mod) {
  thisp->name = name;
  thisp->begln = begln;
  thisp->endln = endln;
  thisp->url = url;
  thisp->descr = descr;
  thisp->mod = mod;
}

void cube_region_construct_cnode(cube_region* thisp) {
  thisp->cnode = (cube_rarray *)malloc(sizeof(cube_rarray));
  if (thisp->cnode == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->cnode->size = 0;
  thisp->cnode->data = (cube_cnode **)malloc(sizeof(cube_cnode *));
  if (thisp->cnode->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->cnode->capacity = 1;
}

void cube_region_add_cnode(cube_region* thisp, cube_cnode* cnode) {
  int i = 0;
  int size = cube_region_num_children(thisp);
  for (i = 0; i < size; i++) {
    if (cube_cnode_equal(cnode, thisp->cnode->data[i]) == 1) return;
  }
  if (thisp->cnode->size < thisp->cnode->capacity) {
    thisp->cnode->data[thisp->cnode->size] = cnode;
    thisp->cnode->size++;
  }
  else if (thisp->cnode->size == thisp->cnode->capacity) {
    thisp->cnode->capacity *= 2;
    thisp->cnode->data = (cube_cnode **)realloc(thisp->cnode->data, sizeof(cube_cnode *)*(thisp->cnode->capacity));
    if (thisp->cnode->data == NULL) { printf("Malloc error\n"); exit(1); }
    thisp->cnode->data[thisp->cnode->size] = cnode;
    thisp->cnode->size++;
  }
}

void cube_region_free(cube_region* thisp) {
  free(thisp->cnode->data);
  free(thisp->cnode);
  if (thisp != NULL) free(thisp);
}

char* cube_region_get_name(cube_region* thisp) {
  return thisp->name;
}

char* cube_region_get_url(cube_region* thisp) {
  return thisp->url;
}

char* cube_region_get_descr(cube_region* thisp) {
  return thisp->descr;
}

char* cube_region_get_mod(cube_region* thisp) {
  return thisp->mod;
}

int cube_region_get_begn_ln(cube_region* thisp) {
  return (thisp->begln <= 0) ? -1 : thisp->begln;
}

int cube_region_get_end_ln(cube_region* thisp) {
  return (thisp->endln <= 0) ? -1 : thisp->endln;
}

int cube_region_num_children(cube_region* thisp) {
  return thisp->cnode->size;
}

void cube_region_writeXML(cube_region* thisp, FILE* fp) {
  char* r = NULL;
  char* s = cube_region_get_name(thisp);
  int len = strlen(s);
  unsigned toks=-1;
  char* p=s;
  /* determine number of tokens to convert */
  while (p <= s+len) {
    size_t n=strcspn(p, "<>&"); /* strcspn(p, "<>&\'\""); */
    p+=(n+1);
    toks++;
  }
  if (toks) r=(char*)calloc(len+(toks*4),1); /* allocate sufficiently long string */
  p=s;
  /* replace each token in new string */
  if (r) while (p <= s+len) {
    size_t n=strcspn(p, "<>&"); /* strcspn(p, "<>&\'\""); */
    strncat(r, p, n);
    p+=n;
    switch(*p) {
      case '<': strcat(r,"lt;"); break;
      case '>': strcat(r,"gt;"); break;
      case '&': strcat(r,"amp;"); break;
#if 0 /* these don't seem required for now */
      case '\'': strcat(r,"apos;"); break;
      case '\"': strcat(r,"quot;"); break;
#endif
      default : break;
    }
    p++;
  }

  fprintf(fp, "    <region id=\"%d\" mod=\"%s\" begin=\"%d\" end=\"%d\">\n",
                   cube_region_get_id(thisp), cube_region_get_mod(thisp), 
                   cube_region_get_begn_ln(thisp), cube_region_get_end_ln(thisp));
  fprintf(fp, "      <name>%s</name>\n", r ? r : s);
  fprintf(fp, "      <url>%s</url>\n", cube_region_get_url(thisp));
  fprintf(fp, "      <descr>%s</descr>\n", cube_region_get_descr(thisp));
  fprintf(fp, "    </region>\n");

  if (r) free(r);
}

int cube_region_equal(cube_region* a, cube_region* b) {
  if (strcmp(cube_region_get_name(a), cube_region_get_name(b)) == 0) {
    if (strcmp(cube_region_get_mod(a), cube_region_get_mod(b)) == 0) 
      return 1;
  }
  return 0;
} 

void cube_region_set_id(cube_region* thisp, int new_id) {
  thisp->id = new_id;
}

int cube_region_get_id(cube_region* thisp) {
  return thisp->id;
}
