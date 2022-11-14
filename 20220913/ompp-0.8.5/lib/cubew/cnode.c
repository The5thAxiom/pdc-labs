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

#include "cnode.h"
#include "region.h"

typedef struct carray {
  int size;
  int capacity;
  struct cube_cnode** data;
} carray;

cube_cnode* cube_cnode_create(cube_cnode* thisp) {
  if (thisp == NULL) {
    thisp = (cube_cnode*) malloc(sizeof(cube_cnode));
    if (thisp == NULL) {
      printf("Malloc error\n");
      exit(1);
    }
  }
  cube_cnode_construct_child(thisp);
  return thisp;
}

void cube_cnode_init(cube_cnode* thisp, struct cube_region* callee, char* mod, int line, cube_cnode* parent) {
  thisp->parent = parent;
  thisp->callee = callee;
  thisp->mod = mod;
  thisp->line = line;
  if (parent != NULL) {
     cube_cnode_add_child(thisp->parent, thisp);
  }
}

void cube_cnode_construct_child(cube_cnode* thisp) {
  thisp->child = (carray *)malloc(sizeof(carray));
  if (thisp->child == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->child->size = 0;
  thisp->child->data = (cube_cnode **)malloc(sizeof(cube_cnode *));
  if (thisp->child->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->child->capacity = 1;
}

void cube_cnode_add_child(cube_cnode* parent, cube_cnode* cnode) {
  if (parent->child->size < parent->child->capacity) {
    parent->child->data[parent->child->size] = cnode;
    parent->child->size++;
  }
  else if (parent->child->size == parent->child->capacity) {
    parent->child->capacity *= 2;
    parent->child->data = (cube_cnode **)realloc(parent->child->data, sizeof(cube_cnode *)*(parent->child->capacity));
    if (parent->child->data == NULL) { printf("Malloc error\n"); exit(1); }
    parent->child->data[parent->child->size] = cnode;
    parent->child->size++;
  }
}

void cube_cnode_free(cube_cnode* thisp) {
  free(thisp->child->data);
  free(thisp->child);
  if (thisp != NULL) free(thisp);
}

cube_cnode* cube_cnode_get_child(cube_cnode* thisp, int i) {
  if (i < 0 || i >= thisp->child->size) {
    printf("cube_cnode_get_child: out of range\n");
  }
  return thisp->child->data[i];
}

cube_cnode* cube_cnode_get_parent(cube_cnode* thisp) {
  return thisp->parent;
}

int cube_cnode_get_line(cube_cnode* thisp) {
  return (thisp->line <= 0) ? -1 : thisp->line;
}

char* cube_cnode_get_mod(cube_cnode* thisp) {
  return thisp->mod;
}

cube_region* cube_cnode_get_callee(cube_cnode* thisp) {
  return thisp->callee;
}

cube_region* cube_cnode_get_caller(cube_cnode* thisp) {
  if (cube_cnode_get_parent(thisp) == NULL) return NULL;
  return cube_cnode_get_callee(cube_cnode_get_parent(thisp));
}

int cube_cnode_num_children(cube_cnode* thisp) {
  return thisp->child->size;
}

void cube_cnode_set_id(cube_cnode* thisp, int new_id) {
  thisp->id = new_id;
}

int cube_cnode_get_id(cube_cnode* thisp) {
  return thisp->id;
}

void cube_cnode_assign_ids(cube_cnode* thisp, int* id) {
  int i = 0;
  int sid = *id;
  cube_cnode_set_id(thisp, sid);
  *id = *id + 1;
  for (i = 0; i < cube_cnode_num_children(thisp); i++) {
    cube_cnode_assign_ids(cube_cnode_get_child(thisp, i), id);
  }
}

int cube_cnode_get_level(cube_cnode* thisp) {
  if (cube_cnode_get_parent(thisp) == NULL)
    return 0;
  else
    return cube_cnode_get_level(cube_cnode_get_parent(thisp)) + 1;
}

void cube_cnode_writeXML(cube_cnode* thisp, FILE* fp) {
  int i, num = cube_cnode_get_level(thisp);

  for (i = 0; i < num; i++) fprintf(fp, "  ");
  fprintf(fp, "    <cnode id=\"%d\" ", cube_cnode_get_id(thisp));
  if (cube_cnode_get_line(thisp) != -1)
    fprintf(fp, "line=\"%d\" ", cube_cnode_get_line(thisp));
  if (strcmp(cube_cnode_get_mod(thisp), "") != 0)
    fprintf(fp, "mod=\"%s\" ", cube_cnode_get_mod(thisp));
  fprintf(fp, "calleeId=\"%d\">\n", cube_region_get_id(cube_cnode_get_callee(thisp)));
  for(i = 0; i < cube_cnode_num_children(thisp) ; i++) {
     cube_cnode* child = cube_cnode_get_child(thisp, i);
     cube_cnode_writeXML(child, fp);
  }
  for (i = 0; i < num; i++) fprintf(fp, "  ");
  fprintf(fp, "    </cnode>\n");
}

int cube_cnode_equal(cube_cnode* a, cube_cnode* b) {
  if (strcmp(cube_cnode_get_mod(a), cube_cnode_get_mod(b)) == 0) {
    if (cube_region_equal(cube_cnode_get_callee(a), cube_cnode_get_callee(b)) == 1) {
      if (cube_cnode_get_line(a) == cube_cnode_get_line(b)) {
        return 1;
      }
    }
  }
  return 0;
}

