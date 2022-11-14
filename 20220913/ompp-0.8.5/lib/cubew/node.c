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
#include "node.h"
#include "process.h"
#include "machine.h"


cube_node* cube_node_create(cube_node* thisp) {
  if (thisp == NULL) {
    thisp = (cube_node*) malloc(sizeof(cube_node));
    if (thisp == NULL) {
      printf("Malloc error\n");
      exit(1);
    }
  }
  cube_node_construct_child(thisp);
  return thisp;
}

void cube_node_init(cube_node* thisp, char* name, cube_machine* parent) {
  thisp->name = name;
  thisp->parent = parent;
  if (parent != NULL) {
    cube_node_add_child(thisp->parent, thisp);
  }
}

void cube_node_construct_child(cube_node* thisp) {
  thisp->child = (cube_parray *)malloc(sizeof(cube_parray));
  if (thisp->child == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->child->size = 0;
  thisp->child->data = (cube_process **)malloc(sizeof(cube_process *));
  if (thisp->child->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->child->capacity = 1;
}

void cube_node_add_child(cube_machine* thisp, cube_node* node) {
  if (thisp->child->size < thisp->child->capacity) {
    thisp->child->data[thisp->child->size] = node;
    thisp->child->size++;
   }
   else if (thisp->child->size == thisp->child->capacity) {
     thisp->child->capacity *= 2;
     thisp->child->data = (cube_node **)realloc(thisp->child->data, sizeof(cube_node *)*(thisp->child->capacity));
     if (thisp->child->data == NULL) { printf("Malloc error\n"); exit(1); }
     thisp->child->data[thisp->child->size] = node;
     thisp->child->size++;
   }
}

void cube_node_free(cube_node* thisp) {
  free(thisp->child->data);
  free(thisp->child);
  if (thisp != NULL) free(thisp);
}

cube_process* cube_node_get_child(cube_node* thisp, int i) {
  if (i < 0 || i >= thisp->child->size) {
    printf("cube_node_get_child: out of range\n");
  }
  return thisp->child->data[i];
}

cube_machine* cube_node_get_parent(cube_node* thisp) {
  return thisp->parent;
}

char* cube_node_get_name(cube_node* thisp) {
  return thisp->name;
}

int cube_node_num_children(cube_node* thisp) {
  return thisp->child->size;
}

int cube_node_get_level(cube_node* thisp) {
  if (cube_node_get_parent(thisp) == NULL)
    return 0;
  else
    return cube_machine_get_level(cube_node_get_parent(thisp)) + 1;
}

void cube_node_writeXML(cube_node* thisp, FILE* fp) {
  int i = 0;
  int num = cube_node_get_level(thisp);
  char ind[80];

  strcpy(ind, "");
  for (i = 0; i < 2*num; i++) {
    strcat(ind, " ");
  }

  fprintf(fp, "%s    <node Id=\"%d\">\n",  ind, cube_node_get_id(thisp));
  fprintf(fp, "%s      <name>%s</name>\n", ind, cube_node_get_name(thisp));
  for(i = 0; i < cube_node_num_children(thisp); i++) {
    cube_process* p = cube_node_get_child(thisp, i);
    cube_process_writeXML(p, fp);
  }
  fprintf(fp, "%s    </node>\n", ind);
}

void cube_node_set_id(cube_node* thisp, int new_id) {
  thisp->id = new_id;
}

int cube_node_get_id(cube_node* thisp) {
  return thisp->id;
}

int cube_node_equal(cube_node* a, cube_node* b) {
  if (strcmp(cube_node_get_name(a), cube_node_get_name(b)) == 0)
    return 1;
  return 0;
}


