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
#include "metric.h"

typedef struct marray {
  int size;
  int capacity;
  struct cube_metric** data;
} marray;


cube_metric* cube_metric_create(cube_metric* thisp) {
  if (thisp == NULL) {
    thisp = (cube_metric*) malloc(sizeof(cube_metric));
    if (thisp == NULL) {
      printf("Malloc error\n");
      exit(1);
    }
  }
  cube_metric_construct_child(thisp);
  return thisp;
}

void cube_metric_init(cube_metric* thisp, char* disp_name, char* uniq_name, char* dtype,
                      char* uom, char* val, char* url, char* descr, cube_metric* parent) {
  thisp->parent = parent;
  thisp->disp_name = disp_name;
  thisp->uniq_name = uniq_name;
  thisp->dtype = dtype;
  thisp->uom = uom;
  thisp->val = val;
  thisp->url = url;
  thisp->descr = descr;
  if (parent != NULL) {
    cube_metric_add_child(thisp->parent, thisp);
  }
}

void cube_metric_construct_child(cube_metric* thisp) {
  thisp->child = (marray *)malloc(sizeof(marray));
  if (thisp->child == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->child->size = 0;
  thisp->child->data = (cube_metric **)malloc(sizeof(cube_metric *));
  if (thisp->child->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->child->capacity = 1;
}

void cube_metric_add_child(cube_metric* parent, cube_metric* met) {
  if (parent->child->size < parent->child->capacity) {
    parent->child->data[parent->child->size] = met;
    parent->child->size++;
  }
  else if (parent->child->size == parent->child->capacity) {
    parent->child->capacity *= 2;
    parent->child->data = (cube_metric **)realloc(parent->child->data, sizeof(cube_metric *)*(parent->child->capacity));
    if (parent->child->data == NULL) { printf("Malloc error\n"); exit(1); }
    parent->child->data[parent->child->size] = met;
    parent->child->size++;
  }
}

void cube_metric_free(cube_metric* thisp) {
  free(thisp->child->data);
  free(thisp->child);
  if (thisp != NULL) free(thisp);
}

cube_metric* cube_metric_get_child(cube_metric* thisp, int i) {
  if (i < 0 || i >= thisp->child->size) {
    printf("cube_metric_get_child: out of range\n");
  }
  return thisp->child->data[i];
}

cube_metric* cube_metric_get_parent(cube_metric* thisp) {
  return thisp->parent;
}

char* cube_metric_get_disp_name(cube_metric* thisp) {
  return thisp->disp_name;
}

char* cube_metric_get_uniq_name(cube_metric* thisp) {
  return thisp->uniq_name;
}

char* cube_metric_get_dtype(cube_metric* thisp) {
  return thisp->dtype;
}

char* cube_metric_get_uom(cube_metric* thisp) {
  return thisp->uom;
}

char* cube_metric_get_val(cube_metric* thisp) {
  return thisp->val;
}

char* cube_metric_get_descr(cube_metric* thisp) {
  return thisp->descr;
}

char* cube_metric_get_url(cube_metric* thisp) {
  return thisp->url;
}

void cube_metric_writeXML(cube_metric* thisp, FILE* fp) {
  int i = 0, num = cube_metric_get_level(thisp);
  char ind[80];

  strcpy(ind, "");
  for (i = 0; i < 2*num; i++) {
    strcat(ind, " ");
  }

  fprintf(fp, "%s    <metric id=\"%d\">\n", ind, cube_metric_get_id(thisp));
  fprintf(fp, "%s      <disp_name>%s</disp_name>\n", ind, cube_metric_get_disp_name(thisp));
  fprintf(fp, "%s      <uniq_name>%s</uniq_name>\n", ind, cube_metric_get_uniq_name(thisp));
  fprintf(fp, "%s      <dtype>%s</dtype>\n", ind, cube_metric_get_dtype(thisp));
  fprintf(fp, "%s      <uom>%s</uom>\n", ind, cube_metric_get_uom(thisp));
  if (strcmp(cube_metric_get_val(thisp), "") != 0) fprintf(fp, "%s      <val>%s</val>\n", ind, cube_metric_get_val(thisp));
  fprintf(fp, "%s      <url>%s</url>\n", ind, cube_metric_get_url(thisp));
  fprintf(fp, "%s      <descr>%s</descr>\n", ind, cube_metric_get_descr(thisp));

  for( i = 0; i < cube_metric_num_children(thisp); i++ ) {
    cube_metric* p = cube_metric_get_child(thisp, i);
    cube_metric_writeXML(p, fp);
  }
  fprintf(fp, "%s    </metric>\n", ind);
}

int cube_metric_num_children(cube_metric* thisp) {
  return thisp->child->size;
}

void cube_metric_assign_ids(cube_metric* thisp, int* id) {
  int i = 0;
  int sid = *id;
  cube_metric_set_id(thisp, sid);
  *id = *id + 1;
  for (i = 0; i < cube_metric_num_children(thisp); i++) {
    cube_metric_assign_ids(cube_metric_get_child(thisp, i), id);
  }
}

int cube_metric_get_level(cube_metric* thisp) {
  if (cube_metric_get_parent(thisp) == NULL)
    return 0;
  else
    return cube_metric_get_level(cube_metric_get_parent(thisp)) + 1;
}

void cube_metric_set_id(cube_metric* thisp, int new_id) { 
  thisp->id = new_id; 
}

int cube_metric_get_id(cube_metric* thisp) { 
  return thisp->id; 
}

int cube_metric_equal(cube_metric* a, cube_metric* b) {
  if (strcmp(cube_metric_get_uniq_name(a), cube_metric_get_uniq_name(b)) == 0)
    return 1;
  return 0;
}

void cube_metric_set_uniq_name(cube_metric* thisp, char* uniq_name) {
  thisp->uniq_name = uniq_name;
}

