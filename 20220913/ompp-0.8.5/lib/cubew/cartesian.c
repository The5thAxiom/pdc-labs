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
#include "cartesian.h"
#include "machine.h"

typedef struct d_arr {
  int size;
  int capacity;
  long int* data;
} d_arr;

typedef struct p_arr {
  int size;
  int capacity;
  int* data;
} p_arr;

typedef struct map_thrd {
  cube_thread* key;
  long int* value;
} map_thrd;

typedef struct array {
  int size;
  int capacity;
  cube_map_thrd** data;
} array;

cube_cartesian* cube_cart_create(cube_cartesian* thisp) {
  if (thisp == NULL) {
    thisp = (cube_cartesian*) malloc(sizeof(cube_cartesian));
    if (thisp == NULL) {
      printf("Malloc error\n");
      exit(1);
    }
  }
  cube_cart_construct_arrays(thisp);
  return thisp;
}

void cube_cart_init(cube_cartesian* thisp, long int ndims, long int* dim, int* period) {
  int i=0;
  thisp->ndims = ndims;
  for (i = 0; i < ndims; i++) {
    cube_cart_add_dim(thisp, dim[i]);
    cube_cart_add_per(thisp, period[i]);
  }
}

void cube_cart_construct_arrays(cube_cartesian* thisp) {
  /* construct array for dims */
  thisp->dim = (d_arr *)malloc(sizeof(d_arr));
  if (thisp->dim == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->dim->size = 0;
  thisp->dim->data = (long int *)malloc(sizeof(long int));
  if (thisp->dim->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->dim->capacity = 1;

  /* construct array for periods */
  thisp->period = (p_arr *)malloc(sizeof(p_arr));
  if (thisp->period == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->period->size = 0;
  thisp->period->data = (int *)malloc(sizeof(int));
  if (thisp->period->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->period->capacity = 1;

  /* construct array for thrd2coord */
  thisp->thrd2coord = (array *)malloc(sizeof(array));
  if (thisp->thrd2coord == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->thrd2coord->size = 0;
  thisp->thrd2coord->data = (map_thrd **)malloc(sizeof(map_thrd *));
  if (thisp->thrd2coord->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->thrd2coord->capacity = 1;
}

void cube_cart_add_dim(cube_cartesian* thisp, long int dim) {
  if (thisp->dim->size < thisp->dim->capacity) {
    thisp->dim->data[thisp->dim->size] = dim;
    thisp->dim->size++;
  }
  else if (thisp->dim->size == thisp->dim->capacity) {
    thisp->dim->capacity *= 2;
    thisp->dim->data = (long int *)realloc(thisp->dim->data, sizeof(long int)*(thisp->dim->capacity));
    if (thisp->dim->data == NULL) { printf("Malloc error\n"); exit(1); }
    thisp->dim->data[thisp->dim->size] = dim;
    thisp->dim->size++;
  }
}

void cube_cart_add_per(cube_cartesian* thisp, int period) {
  if (thisp->period->size < thisp->period->capacity) {
    thisp->period->data[thisp->period->size] = period;
    thisp->period->size++;
  }
  else if (thisp->period->size == thisp->period->capacity) {
    thisp->period->capacity *= 2;
    thisp->period->data = (int *)realloc(thisp->period->data, sizeof(int)*(thisp->period->capacity));
    if (thisp->period->data == NULL) { printf("Malloc error\n"); exit(1); }
    thisp->period->data[thisp->period->size] = period;
    thisp->period->size++;
  }
}

void cube_cart_add_thrd2coord(cube_cartesian* thisp, map_thrd *m) {
  if (thisp->thrd2coord->size < thisp->thrd2coord->capacity) {
    thisp->thrd2coord->data[thisp->thrd2coord->size] = m;
    thisp->thrd2coord->size++;
  }
  else if (thisp->thrd2coord->size == thisp->thrd2coord->capacity) {
    thisp->thrd2coord->capacity *= 2;
    thisp->thrd2coord->data = (map_thrd **)realloc(thisp->thrd2coord->data, sizeof(map_thrd *)*(thisp->thrd2coord->capacity));
    if (thisp->thrd2coord->data == NULL) { printf("Malloc error\n"); exit(1); }
    thisp->thrd2coord->data[thisp->thrd2coord->size] = m;
    thisp->thrd2coord->size++;
  }
}

void cube_cart_free(cube_cartesian* thisp) {
  int i = 0;
  free(thisp->dim->data);
  free(thisp->dim);
  free(thisp->period->data);
  free(thisp->period);
  for (i = 0; i < thisp->thrd2coord->size; i++) {
    free(thisp->thrd2coord->data[i]->value);
    free(thisp->thrd2coord->data[i]);
  }
  free(thisp->thrd2coord->data);
  free(thisp->thrd2coord);
  if(thisp != NULL) free(thisp);
}

void cube_cart_def_coords(cube_cartesian* thisp, cube_thread* thrd, long int *coord) {
  int i = 0;
  map_thrd* m = (map_thrd *)malloc(sizeof(map_thrd));
  if (m == NULL) { printf("Malloc error\n"); exit(1); }
  cube_cart_add_thrd2coord(thisp, m);

  m->value = (long int *)malloc(sizeof(long int)*thisp->ndims);
  if (m->value == NULL) {
    printf("Malloc error\n"); exit(1);
  }
  m->key = thrd;
  for (i = 0; i < thisp->ndims; i++) {
    m->value[i] = coord[i];
  }
}

void cube_cart_writeXML(cube_cartesian* thisp, FILE* fp) {
  typedef struct map {
    int key;
    cube_thread* value;
  } map;

  int i = 0, j = 0, k = 0;
  const char* indent = "    ";
  const char* per;
  map *tid_order = (map*) malloc(thisp->thrd2coord->size * sizeof(map));
  long int *pt = (long int*) malloc(thisp->ndims * sizeof(long int));

  fprintf (fp, "%s  <cart ndims=\"%ld\">\n", indent, thisp->ndims);

  if (thisp->ndims != thisp->dim->size || thisp->ndims != thisp->period->size) {
    printf("Cartesian: inconsistent dimensions defined!");
    exit(1);
  }

  for (i = 0; i < thisp->ndims; i++) {
    if (thisp->period->data[i] == 0) per = "false";
    else per = "true";
    fprintf(fp, "%s    <dim size=\"%ld\" periodic=\"%s\"/>\n", indent, thisp->dim->data[i], per);
  }

  for (i = 0; i < thisp->thrd2coord->size; i++) {
    cube_thread* thrd = thisp->thrd2coord->data[i]->key; 
    tid_order[i].key = cube_thread_get_id(thrd);
    tid_order[i].value = thrd;
  }

  for (j = 0; j < thisp->thrd2coord->size; j++) {
    if (cube_thread_equal(tid_order[j].value, thisp->thrd2coord->data[j]->key) == 1) {
      int p = 0;
      cube_thread* t = thisp->thrd2coord->data[j]->key;
      for (p = 0; p < thisp->ndims; p++) {
        pt[p] = thisp->thrd2coord->data[j]->value[p];
      }
      fprintf(fp, "%s    <coord thrdId=\"%d\">", indent, cube_thread_get_id(t)); 
      for (k = 0; k < thisp->ndims; k++) { 
        if (k < thisp->ndims-1) fprintf(fp, "%ld ", pt[k]);
        else fprintf(fp, "%ld", pt[k]); 
      }
      fprintf(fp, "</coord>\n"); 
    }
  }
  fprintf(fp, "%s  </cart>\n", indent);
  free(tid_order);
  free(pt);
}
