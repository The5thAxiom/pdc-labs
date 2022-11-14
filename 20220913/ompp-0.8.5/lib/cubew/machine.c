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

#include "machine.h"
#include "node.h"


cube_machine* cube_machine_create(cube_machine* thisp) {
  if (thisp == NULL) {
    thisp = (cube_machine*) malloc(sizeof(cube_machine));
    if (thisp == NULL) {
      printf("Malloc error\n");
      exit(1);
    }
  }
  cube_machine_construct_child(thisp);
  return thisp;
}

void cube_machine_init(cube_machine* thisp, char* name, char* desc) {
  thisp->name = name;
  thisp->desc = desc;
}

void cube_machine_construct_child(cube_machine* thisp) {
   thisp->child = (cube_narray *)malloc(sizeof(cube_narray));
   if (thisp->child == NULL) { printf("Malloc error\n"); exit(1); }
   thisp->child->size = 0;
   thisp->child->data = (cube_node **)malloc(sizeof(cube_node *));
   if (thisp->child->data == NULL) { printf("Malloc error\n"); exit(1); }
   thisp->child->capacity = 1;
}

void cube_machine_free(cube_machine* thisp) {
  free(thisp->child->data);
  free(thisp->child);
  if (thisp != NULL) free(thisp);
}

cube_node* cube_machine_get_child(cube_machine* thisp, int i) {
  if (i < 0 || i >= thisp->child->size) {
    printf("cube_machine_get_child: out of range\n");
  }
  return thisp->child->data[i];
}

char* cube_machine_get_name(cube_machine* thisp) {
  return thisp->name;
}

char* cube_machine_get_desc(cube_machine* thisp) {
  return thisp->desc;
}

int cube_machine_num_children(cube_machine* thisp) {
  return thisp->child->size;
}

int cube_machine_get_level(cube_machine* thisp) {
  return 0;
}

void cube_machine_writeXML(cube_machine* thisp, FILE* fp) {
  int i = 0, num = cube_machine_get_level(thisp);
  char ind[80];

  strcpy(ind, "");
  for (i = 0; i < 2*num; i++) {
    strcat(ind, " ");
  }

  fprintf(fp, "%s    <machine Id=\"%d\">\n", ind, cube_machine_get_id(thisp));
  fprintf(fp, "%s      <name>%s</name>\n", ind, cube_machine_get_name(thisp));
  if (strcmp(cube_machine_get_desc(thisp), "") != 0) fprintf(fp, "%s      <descr>%s</descr>\n", ind, cube_machine_get_desc(thisp));
  for (i = 0; i < cube_machine_num_children(thisp); i++) {
    cube_node* node = cube_machine_get_child(thisp, i);
    cube_node_writeXML(node, fp);
  }
  fprintf(fp, "%s    </machine>\n", ind);
}

void cube_machine_set_id(cube_machine* thisp, int new_id) {
  thisp->id = new_id;
}

int cube_machine_get_id(cube_machine* thisp) {
  return thisp->id;
}

int cube_machine_equal(cube_machine* a, cube_machine* b) {
  if (strcmp(cube_machine_get_name(a), cube_machine_get_name(b)) == 0) 
    return 1;
  return 0;
}

