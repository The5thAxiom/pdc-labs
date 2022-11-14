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
#include "thread.h"
#include "process.h"


cube_thread* cube_thread_create(cube_thread* thisp) {
  if (thisp == NULL) {
    thisp = (cube_thread*) malloc(sizeof(cube_thread));
    if (thisp == NULL) {
      printf("Malloc error\n");
      exit(1);
    }
  }
  cube_thread_construct_child(thisp);
  return thisp;
}

void cube_thread_init(cube_thread* thisp, char* name, int rank, cube_process* parent) {
  thisp->name = name;
  thisp->rank = rank;
  thisp->parent = parent;
  if (parent != NULL) {
    cube_thread_add_child(thisp->parent, thisp);
  }
}

void cube_thread_construct_child(cube_thread* thisp) {
  thisp->child = (cube_tarray *)malloc(sizeof(cube_tarray));
  if (thisp->child == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->child->size = 0;
  thisp->child->data = (cube_thread **)malloc(sizeof(cube_thread *));
  if (thisp->child->data == NULL) { printf("Malloc error\n"); exit(1); }
    thisp->child->capacity = 1;
}

void cube_thread_add_child(cube_process* thisp, cube_thread* thrd) {
  if (thisp->child->size < thisp->child->capacity) {
    thisp->child->data[thisp->child->size] = thrd;
    thisp->child->size++;
  }
  else if (thisp->child->size == thisp->child->capacity) {
    thisp->child->capacity *= 2;
    thisp->child->data = (cube_thread **)realloc(thisp->child->data, sizeof(cube_thread *)*(thisp->child->capacity));
    if (thisp->child->data == NULL) { printf("Malloc error\n"); exit(1); }
    thisp->child->data[thisp->child->size] = thrd;
    thisp->child->size++;
  }
}

void cube_thread_free(cube_thread* thisp) {
  free(thisp->child->data);
  free(thisp->child);
  if (thisp != NULL) free(thisp);
}

cube_thread* cube_thread_get_child(cube_thread* thisp, int i) {
  if (i < 0 || i >= thisp->child->size) {
    printf("cube_thread_get_child: out of range\n");
  }
  return thisp->child->data[i];
}

cube_process* cube_thread_get_parent(cube_thread* thisp) {
  return thisp->parent;
}

char* cube_thread_get_name(cube_thread* thisp) {
  return thisp->name;
}

int cube_thread_get_rank(cube_thread* thisp) {
  return thisp->rank;
}

int cube_thread_num_children(cube_thread* thisp) {
  return thisp->child->size;
}

int cube_thread_get_level(cube_thread* thisp) {
  if (cube_thread_get_parent(thisp) == NULL)
    return 0;
  else
    return cube_process_get_level(cube_thread_get_parent(thisp)) + 1;
}

void cube_thread_writeXML(cube_thread* thisp, FILE* fp) {
  char ind[80];
  int i = 0, num = cube_thread_get_level(thisp);

  strcpy(ind, "");
  for (i = 0; i < 2*num; i++) {
    strcat(ind, " ");
   }

  fprintf(fp, "%s    <thread Id=\"%d\">\n", ind, cube_thread_get_id(thisp));
  fprintf(fp, "%s      <name>%s</name>\n", ind, cube_thread_get_name(thisp));
  fprintf(fp, "%s      <rank>%d</rank>\n", ind, cube_thread_get_rank(thisp));
  fprintf(fp, "%s    </thread>\n", ind);
}

void cube_thread_set_id(cube_thread* thisp, int new_id) {
  thisp->id = new_id;
}

int cube_thread_get_id(cube_thread* thisp) {
  return thisp->id;
}

int cube_thread_equal(cube_thread* a, cube_thread* b) {
  if (cube_thread_get_rank(a) == cube_thread_get_rank(b)) {
    if (cube_process_get_rank(cube_thread_get_parent(a)) == cube_process_get_rank(cube_thread_get_parent(b))) 
      return 1;
  }
  return 0;
}


