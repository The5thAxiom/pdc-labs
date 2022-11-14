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
#include "process.h"
#include "node.h"


cube_process* cube_process_create(cube_process* thisp) {
  if (thisp == NULL) {
    thisp = (cube_process*) malloc(sizeof(cube_process));
    if (thisp == NULL) {
      printf("Malloc error\n");
      exit(1);
    }
  }
  cube_process_construct_child(thisp);
  return thisp;
}

void cube_process_init(cube_process* thisp, char* name, int rank, cube_node* parent) {
  thisp->name = name;
  thisp->rank = rank;
  thisp->parent = parent;
  if (parent != NULL) {
    cube_process_add_child(thisp->parent, thisp);
  }
}

void cube_process_construct_child(cube_process* thisp) {
  thisp->child = (cube_tarray *)malloc(sizeof(cube_tarray));
  if (thisp->child == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->child->size = 0;
  thisp->child->data = (cube_thread **)malloc(sizeof(cube_thread *));
  if (thisp->child->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->child->capacity = 1;
}

void cube_process_add_child(cube_node* thisp, cube_process* proc) {
  if (thisp->child->size < thisp->child->capacity) {
    thisp->child->data[thisp->child->size] = proc;
    thisp->child->size++;
  }
  else if (thisp->child->size == thisp->child->capacity) {
    thisp->child->capacity *= 2;
    thisp->child->data = (cube_process **)realloc(thisp->child->data, sizeof(cube_process *)*(thisp->child->capacity));
    if (thisp->child->data == NULL) { printf("Malloc error\n"); exit(1); }
    thisp->child->data[thisp->child->size] = proc;
    thisp->child->size++;
  }
}

void cube_process_free(cube_process* thisp) {
  free(thisp->child->data);
  free(thisp->child);
  if (thisp != NULL) free(thisp);
}

cube_thread* cube_process_get_child(cube_process* thisp, int i) {
  if (i < 0 || i >= thisp->child->size) {
    printf("cube_process_get_child: out of range\n");
  }
  return thisp->child->data[i];
}

cube_node* cube_process_get_parent(cube_process* thisp) {
  return thisp->parent;
}

int cube_process_get_rank(cube_process* thisp) {
  return thisp->rank;
}

char* cube_process_get_name(cube_process* thisp) {
  return thisp->name;
}

int cube_process_num_children(cube_process* thisp) {
  return thisp->child->size;
}

int cube_process_get_level(cube_process* thisp) {
  if (cube_process_get_parent(thisp) == NULL)
    return 0;
  else
    return cube_node_get_level(cube_process_get_parent(thisp)) + 1;
}

void cube_process_writeXML(cube_process* thisp, FILE* fp) {
  int i = 0, num = cube_process_get_level(thisp);
  char ind[80];

  strcpy(ind, "");
  for (i = 0; i < 2*num; i++) {
    strcat(ind, " ");
  }

  fprintf(fp, "%s    <process Id=\"%d\">\n", ind, cube_process_get_id(thisp));
  fprintf(fp, "%s      <name>%s</name>\n", ind, cube_process_get_name(thisp));
  fprintf(fp, "%s      <rank>%d</rank>\n", ind, cube_process_get_rank(thisp));
  for(i = 0; i < cube_process_num_children(thisp); i++) {
    cube_thread* thrd = cube_process_get_child(thisp, i);
    cube_thread_writeXML(thrd, fp);
  }
  fprintf(fp, "%s    </process>\n", ind);
}

void cube_process_set_id(cube_process* thisp, int new_id) {
  thisp->id = new_id;
}

int cube_process_get_id(cube_process* thisp) {
  return thisp->id;
}

int cube_process_equal(cube_process* a, cube_process* b) {
  if (cube_process_get_rank(a) == cube_process_get_rank(b))
    return 1;
  return 0;
}


