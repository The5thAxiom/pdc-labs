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

/************************************************
 
             cube.c

************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cube.h"

typedef struct dyn_array {
  int size;
  int capacity;
  void** data;
} dyn_array;

typedef struct cmap {
  char* key;
  char* value;
} cmap;

typedef struct att_array {
  int size;
  int capacity;
  cmap** data;
} att_array;

cube_t* cube_create() {
  /* allocate memory for cube */
  cube_t* thisp = (cube_t *) calloc(1,sizeof(cube_t));
  if (thisp == NULL) {
    printf("Malloc error\n");
    exit(1);
  }
  /* construct dynamic arrays */
  cube_construct_arrays(thisp);
  thisp->sev_flag = 1;
  return thisp;
}

void cube_sev_init(cube_t* thisp) {
  int i = 0, j = 0, k = 0;
  thisp->sev = (double ***)calloc(thisp->met_ar->size, sizeof(double **));
  if (thisp->sev == NULL) { printf("Malloc error\n"); exit(1); }
  for (i = 0; i < thisp->met_ar->size; i++) {
    thisp->sev[i] = (double **)calloc(thisp->cnd_ar->size, sizeof(double *));
    if (thisp->sev[i] == NULL) { printf("Malloc error\n"); exit(1); }
  }

  thisp->exist = (int **)calloc(thisp->met_ar->size, sizeof(int *));
  for (i = 0; i < thisp->met_ar->size; i++) {
    thisp->exist[i] = (int *)calloc(thisp->cnd_ar->size, sizeof(int));
    if (thisp->exist[i] == NULL) { printf("Malloc error\n"); exit(1); }
  }
  thisp->cn_exist = (int ***)calloc(thisp->met_ar->size, sizeof(int **));
  if (thisp->cn_exist == NULL) { printf("Malloc error\n"); exit(1); }
  for (i = 0; i < thisp->met_ar->size; i++) {
    thisp->cn_exist[i] = (int **)calloc(thisp->cnd_ar->size, sizeof(int *));
    if (thisp->cn_exist[i] == NULL) { printf("Malloc error\n"); exit(1); }
    for (j = 0; j < thisp->cnd_ar->size; j++) {
      thisp->cn_exist[i][j] = (int *)calloc(thisp->thrd_ar->size, sizeof(int));
      if (thisp->cn_exist[i][j] == NULL) { printf("Malloc error\n"); exit(1); }
    }
  } 
  cube_assign_ids(thisp);
}

void cube_free(cube_t* thisp) {
  int i = 0, j = 0;
  if (!thisp) return;
  /* free severities */
  if (thisp->sev) {
    for (i = 0; i < thisp->met_ar->size; i++) {
      for (j = 0; j < thisp->cnd_ar->size; j++) {
        free(thisp->sev[i][j]);
      }
      free(thisp->sev[i]);
    }
    free(thisp->sev);
  }
  if (thisp->exist) {
    for (i = 0; i < thisp->met_ar->size; i++) {
      free(thisp->exist[i]);
    }
    free(thisp->exist);
  }
  if (thisp->cn_exist) {
    for (i = 0; i < thisp->met_ar->size; i++) {
      for (j = 0; j < thisp->cnd_ar->size; j++)
        free(thisp->cn_exist[i][j]);
      free(thisp->cn_exist[i]);
    }
    free(thisp->cn_exist);
  }
  /* free metrics */
  if (thisp->met_ar) {
    for (i = 0; i < thisp->met_ar->size; i++) {
      cube_metric_free((cube_metric*)(thisp->met_ar->data[i])); 
    }
    free(thisp->met_ar->data);
    free(thisp->met_ar);
  }
  if (thisp->rmet_ar) {
    free(thisp->rmet_ar->data);
    free(thisp->rmet_ar);
  }
  /* free regions */
  if (thisp->reg_ar) {
    for (i = 0; i < thisp->reg_ar->size; i++) {
      cube_region_free((cube_region*)(thisp->reg_ar->data[i]));
    }
    free(thisp->reg_ar->data);
    free(thisp->reg_ar);
  }
  /* free cnodes */
  if (thisp->cnd_ar) {
    for (i = 0; i < thisp->cnd_ar->size; i++) {
      cube_cnode_free((cube_cnode*)(thisp->cnd_ar->data[i]));
    }
    free(thisp->cnd_ar->data);
    free(thisp->cnd_ar);
  }
  if (thisp->rcnd_ar) {
    free(thisp->rcnd_ar->data);
    free(thisp->rcnd_ar);
  }
  /* free machines */
  if (thisp->mach_ar) {
    for (i = 0; i < thisp->mach_ar->size; i++) {
      cube_machine_free((cube_machine*)(thisp->mach_ar->data[i]));
    }
    free(thisp->mach_ar->data);
    free(thisp->mach_ar);
  }
  /* free nodes */
  if (thisp->node_ar) {
    for (i = 0; i < thisp->node_ar->size; i++) {
      cube_node_free((cube_node*)(thisp->node_ar->data[i]));
    }
    free(thisp->node_ar->data);
    free(thisp->node_ar);
  }
  /* free processes */
  if (thisp->proc_ar) {
    for (i = 0; i < thisp->proc_ar->size; i++) {
      cube_process_free((cube_process*)(thisp->proc_ar->data[i]));
    }
    free(thisp->proc_ar->data);
    free(thisp->proc_ar);
  }
  /* free threads */
  if (thisp->thrd_ar) {
    for (i = 0; i < thisp->thrd_ar->size; i++) {
      cube_thread_free((cube_thread*)(thisp->thrd_ar->data[i]));
    }
    free(thisp->thrd_ar->data);
    free(thisp->thrd_ar);
  }
  /* free attrs */
  if (thisp->attr) {
    for (i = 0; i < thisp->attr->size; i++) {
      free(thisp->attr->data[i]); 
    }
    free(thisp->attr->data);
    free(thisp->attr);
  }
  /* free mirrors */
  if (thisp->mirr_ar) {
    free(thisp->mirr_ar->data);
    free(thisp->mirr_ar);
  }
  /* free cartesians */
  for (i = 0; i < thisp->cart_ar->size; i++) {
    free(thisp->cart_ar->data[i]); 
  }
  if (thisp->cart_ar) {
    free(thisp->cart_ar->data);
    free(thisp->cart_ar);
  }
  /* lastly, delete the cube object */
  free(thisp); 
}

void cube_construct_arrays(cube_t* thisp) {
  /* construct metric array */
  thisp->met_ar = (dyn_array *)calloc(1, sizeof(dyn_array));
  if (thisp->met_ar == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->met_ar->size = 0; 
  thisp->met_ar->data = (void **)calloc(1, sizeof(void *));
  if (thisp->met_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->met_ar->capacity = 1;

  /* construct root metric array */
  thisp->rmet_ar = (dyn_array *)calloc(1, sizeof(dyn_array));
  if (thisp->rmet_ar == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->rmet_ar->size = 0; 
  thisp->rmet_ar->data = (void **)calloc(1, sizeof(void *));
  if (thisp->rmet_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->rmet_ar->capacity = 1;

  /* construct region array */
  thisp->reg_ar = (dyn_array *)calloc(1, sizeof(dyn_array));
  if (thisp->reg_ar == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->reg_ar->size = 0; 
  thisp->reg_ar->data = (void **)calloc(1, sizeof(void *));
  if (thisp->reg_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->reg_ar->capacity = 1;

  /* construct cnode array */
  thisp->cnd_ar = (dyn_array *)calloc(1, sizeof(dyn_array));
  if (thisp->cnd_ar == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->cnd_ar->size = 0; 
  thisp->cnd_ar->data = (void **)calloc(1, sizeof(void *));
  if (thisp->cnd_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->cnd_ar->capacity = 1;

  /* construct root cnode array */
  thisp->rcnd_ar = (dyn_array *)calloc(1, sizeof(dyn_array));
  if (thisp->rcnd_ar == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->rcnd_ar->size = 0; 
  thisp->rcnd_ar->data = (void **)calloc(1, sizeof(void *));
  if (thisp->rcnd_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->rcnd_ar->capacity = 1;

  /* construct machine array */
  thisp->mach_ar = (dyn_array *)calloc(1, sizeof(dyn_array)*10);
  if (thisp->mach_ar == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->mach_ar->size = 0; 
  thisp->mach_ar->data = (void **)calloc(1, sizeof(void *));
  if (thisp->mach_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->mach_ar->capacity = 1;

  /* construct node array */
  thisp->node_ar = (dyn_array *)calloc(1, sizeof(dyn_array));
  if (thisp->node_ar == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->node_ar->size = 0; 
  thisp->node_ar->data = (void **)calloc(1, sizeof(void *));
  if (thisp->node_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->node_ar->capacity = 1;

  /* construct process array */
  thisp->proc_ar = (dyn_array *)calloc(1, sizeof(dyn_array));
  if (thisp->proc_ar == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->proc_ar->size = 0; 
  thisp->proc_ar->data = (void **)calloc(1, sizeof(void *));
  if (thisp->proc_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->proc_ar->capacity = 1;

  /* construct thread array */
  thisp->thrd_ar = (dyn_array *)calloc(1, sizeof(dyn_array));
  if (thisp->thrd_ar == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->thrd_ar->size = 0; 
  thisp->thrd_ar->data = (void **)calloc(1, sizeof(void *));
  if (thisp->thrd_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->thrd_ar->capacity = 1;

  /* construct cartesian array */
  thisp->cart_ar = (dyn_array *)calloc(1, sizeof(dyn_array));
  if (thisp->cart_ar == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->cart_ar->size = 0; 
  thisp->cart_ar->data = (void **)calloc(1, sizeof(void *));
  if (thisp->cart_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->cart_ar->capacity = 1;

  /* construct mirror_url array */
  thisp->mirr_ar = (dyn_array *)calloc(1, sizeof(dyn_array));
  if (thisp->mirr_ar == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->mirr_ar->size = 0; 
  thisp->mirr_ar->data = (void **)calloc(1, sizeof(void *));
  if (thisp->mirr_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->mirr_ar->capacity = 1;

  /* construct attrs array */
  thisp->attr = (att_array *)calloc(1, sizeof(att_array));
  if (thisp->attr == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->attr->size = 0; 
  thisp->attr->data = (cmap **)calloc(1, sizeof(cmap *));
  if (thisp->attr->data == NULL) { printf("Malloc error\n"); exit(1); }
  thisp->attr->capacity = 1;
}

void cube_add_metric(cube_t* thisp, cube_metric* met) {
  if (thisp->met_ar->size == thisp->met_ar->capacity) {
    thisp->met_ar->capacity *= 2;
    thisp->met_ar->data = (void **)realloc(thisp->met_ar->data, sizeof(void *)*(thisp->met_ar->capacity));
    if (thisp->met_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  }
  thisp->met_ar->data[thisp->met_ar->size] = met;
  thisp->met_ar->size++;
}

void cube_add_rmetric(cube_t* thisp, cube_metric* met) {
  if (thisp->rmet_ar->size == thisp->rmet_ar->capacity) {
    thisp->rmet_ar->capacity *= 2;
    thisp->rmet_ar->data = (void **)realloc(thisp->rmet_ar->data, sizeof(void *)*(thisp->rmet_ar->capacity));
    if (thisp->rmet_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  }
  thisp->rmet_ar->data[thisp->rmet_ar->size] = met;
  thisp->rmet_ar->size++;
}

void cube_add_cnode(cube_t* thisp, cube_cnode* cnd) {
  if (thisp->cnd_ar->size == thisp->cnd_ar->capacity) {
    thisp->cnd_ar->capacity *= 2;
    thisp->cnd_ar->data = (void **)realloc(thisp->cnd_ar->data, sizeof(void *)*(thisp->cnd_ar->capacity));
    if (thisp->cnd_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  }
  thisp->cnd_ar->data[thisp->cnd_ar->size] = cnd;
  thisp->cnd_ar->size++;
}

void cube_add_rcnode(cube_t* thisp, cube_cnode* cnd) {
  if (thisp->rcnd_ar->size == thisp->rcnd_ar->capacity) {
    thisp->rcnd_ar->capacity *= 2;
    thisp->rcnd_ar->data = (void **)realloc(thisp->rcnd_ar->data, sizeof(void *)*(thisp->rcnd_ar->capacity));
    if (thisp->rcnd_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  }
  thisp->rcnd_ar->data[thisp->rcnd_ar->size] = cnd;
  thisp->rcnd_ar->size++;
}

void cube_add_region(cube_t* thisp, cube_region* reg) {
  if (thisp->reg_ar->size == thisp->reg_ar->capacity) {
    thisp->reg_ar->capacity *= 2;
    thisp->reg_ar->data = (void **)realloc(thisp->reg_ar->data, sizeof(void *)*(thisp->reg_ar->capacity));
    if (thisp->reg_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  }
  thisp->reg_ar->data[thisp->reg_ar->size] = reg;
  thisp->reg_ar->size++;
}

void cube_add_mach(cube_t* thisp, cube_machine* mach) {
  if (thisp->mach_ar->size == thisp->mach_ar->capacity) {
    thisp->mach_ar->capacity *= 2;
    thisp->mach_ar->data = (void **)realloc(thisp->mach_ar->data, sizeof(void *)*(thisp->mach_ar->capacity));
    if (thisp->mach_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  }
  thisp->mach_ar->data[thisp->mach_ar->size] = mach;
  thisp->mach_ar->size++;
}

void cube_add_node(cube_t* thisp, cube_node* node) {
  if (thisp->node_ar->size == thisp->node_ar->capacity) {
    thisp->node_ar->capacity *= 2;
    thisp->node_ar->data = (void **)realloc(thisp->node_ar->data, sizeof(void *)*(thisp->node_ar->capacity));
    if (thisp->node_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  }
  thisp->node_ar->data[thisp->node_ar->size] = node;
  thisp->node_ar->size++;
}

void cube_add_proc(cube_t* thisp, cube_process* proc) {
  if (thisp->proc_ar->size == thisp->proc_ar->capacity) {
    thisp->proc_ar->capacity *= 2;
    thisp->proc_ar->data = (void **)realloc(thisp->proc_ar->data, sizeof(void *)*(thisp->proc_ar->capacity));
    if (thisp->proc_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  }
  thisp->proc_ar->data[thisp->proc_ar->size] = proc;
  thisp->proc_ar->size++;
}

void cube_add_thrd(cube_t* thisp, cube_thread* thrd) {
  if (thisp->thrd_ar->size == thisp->thrd_ar->capacity) {
    thisp->thrd_ar->capacity *= 2;
    thisp->thrd_ar->data = (void **)realloc(thisp->thrd_ar->data, sizeof(void *)*(thisp->thrd_ar->capacity));
    if (thisp->thrd_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  }
  thisp->thrd_ar->data[thisp->thrd_ar->size] = thrd;
  thisp->thrd_ar->size++;
}

void cube_add_cart(cube_t* thisp, cube_cartesian* cart) {
  if (thisp->cart_ar->size == thisp->cart_ar->capacity) {
    thisp->cart_ar->capacity *= 2;
    thisp->cart_ar->data = (void **)realloc(thisp->cart_ar->data, sizeof(void *)*(thisp->cart_ar->capacity));
    if (thisp->cart_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  }
  thisp->cart_ar->data[thisp->cart_ar->size] = cart;
  thisp->cart_ar->size++;
}

void cube_add_mirror(cube_t* thisp, const char* mir) {
  char* nmir = strdup(mir);
  if (thisp->mirr_ar->size == thisp->mirr_ar->capacity) {
    thisp->mirr_ar->capacity *= 2;
    thisp->mirr_ar->data = (void **)realloc(thisp->mirr_ar->data, sizeof(void *)*(thisp->mirr_ar->capacity));
    if (thisp->mirr_ar->data == NULL) { printf("Malloc error\n"); exit(1); }
  }
  thisp->mirr_ar->data[thisp->mirr_ar->size] = nmir;
  thisp->mirr_ar->size++;
}

void cube_add_attr(cube_t* thisp, cmap* m) {
  if (thisp->attr->size == thisp->attr->capacity) {
    thisp->attr->capacity *= 2;
    thisp->attr->data = (cmap **)realloc(thisp->attr->data, sizeof(cmap *)*(thisp->attr->capacity));
    if (thisp->attr->data == NULL) { printf("Malloc error\n"); exit(1); }
  }
  thisp->attr->data[thisp->attr->size] = m;
  thisp->attr->size++;
}

void cube_def_attr(cube_t* thisp, const char* key, const char* value) {
  char* nkey = strdup(key);
  char* nvalue = strdup(value);
  cmap* m = (cmap *)calloc(1, sizeof(cmap));
  if (m == NULL) { printf("Malloc error\n"); exit(1); }
  m->key = nkey;
  m->value = nvalue;
  cube_add_attr(thisp, m); 
}

void cube_def_mirror(cube_t* thisp, const char* url) {
  char* nurl = strdup(url);
  cube_add_mirror(thisp, nurl);
}

cube_metric* cube_def_met(cube_t* thisp, const char* disp_name, const char* uniq_name, const char* dtype, const char* uom, 
     		          const char* val, const char* url, const char* descr, cube_metric* parent) { 
  char* ndisp = strdup(disp_name);
  char* nuniq = strdup(uniq_name);
  char* ndtype = strdup(dtype);
  char* nuom = strdup(uom);
  char* nval = strdup(val);
  char* nurl = strdup(url);
  char* ndescr = strdup(descr);
  cube_metric* met = cube_metric_create(NULL); 
  cube_metric_init(met, ndisp, nuniq, ndtype, nuom, nval, nurl, ndescr, parent);
  if (parent == NULL) {
    cube_add_rmetric(thisp, met);
  }
  cube_add_metric(thisp, met);
  return met;
}

cube_region* cube_def_region(cube_t* thisp, const char* name, long begln, long endln, 
		        const char* url, const char* descr, const char* mod) {
  char* nname = strdup(name);
  char* nurl = strdup(url);
  char* ndescr = strdup(descr);
  char* nmod = strdup(mod);
  cube_region* reg = cube_region_create(NULL); 
  cube_region_init(reg, nname, begln, endln, nurl, ndescr, nmod);
  cube_add_region(thisp, reg);
  return reg;
}

cube_cnode* cube_def_cnode_cs(cube_t* thisp, cube_region *callee, const char* mod, int line, cube_cnode* parent) {
  char* nmod = strdup(mod);
  cube_cnode* cnode = cube_cnode_create(NULL); 
  cube_cnode_init(cnode, callee, nmod, line, parent);
  if (parent == NULL) {
    cube_add_rcnode(thisp, cnode);
  }
  else {
    cube_region* caller = cube_cnode_get_callee(parent);
    cube_region_add_cnode(caller, cnode);
  }
  cube_add_cnode(thisp, cnode);
  return cnode;
}

cube_cnode* cube_def_cnode(cube_t* thisp, cube_region* callee, cube_cnode* parent) {
  /* if source code info not available, use empty string for mod */
  /* and -1 for line # */
  cube_cnode* cnode = cube_cnode_create(NULL); 
  cube_cnode_init(cnode, callee, "", 0, parent);
  if (parent == NULL) {
    cube_add_rcnode(thisp, cnode);
  }
  else { 
    cube_region* caller = cube_cnode_get_callee(parent);
    cube_region_add_cnode(caller, cnode);
  }
  cube_add_cnode(thisp, cnode);
  return cnode;
}

cube_machine* cube_def_mach(cube_t* thisp, const char* name, const char* desc) {
  char* nname = strdup(name);
  char* ndesc = strdup(desc);
  cube_machine* mach = cube_machine_create(NULL); 
  cube_machine_init(mach, nname, ndesc);
  cube_add_mach(thisp, mach);
  return mach;
}

cube_node* cube_def_node(cube_t* thisp, const char* name, cube_machine* mach) {
  char* nname = strdup(name);
  cube_node* node = cube_node_create(NULL); 
  cube_node_init(node, nname, mach);
  cube_add_node(thisp, node);
  return node;
}

cube_process* cube_def_proc(cube_t* thisp, const char* name, int rank, cube_node* node) {
  char* nname = strdup(name);
  cube_process* proc = cube_process_create(NULL); 
  cube_process_init(proc, nname, rank, node);
  cube_add_proc(thisp, proc);
  return proc;
}

cube_thread* cube_def_thrd(cube_t* thisp, const char* name, int rank, cube_process* proc) {
  char* nname = strdup(name);
  cube_thread* thrd = cube_thread_create(NULL); 
  cube_thread_init(thrd, nname, rank, proc);
  cube_add_thrd(thisp, thrd);
  return thrd;
}

cube_cartesian* cube_def_cart(cube_t* thisp, long int ndims, long int* dim, int* period) {
  cube_cartesian* newc;
  if (ndims > 3) {
    fprintf(stderr,"cubew: WARNING: CUBE3 doesn't support Cartesian topologies with > 3 dimensions\n");
    return NULL;
  }
  newc = cube_cart_create(NULL);
  cube_cart_init(newc, ndims, dim, period);
  cube_add_cart(thisp, newc);
  return newc;
}

void cube_def_coords(cube_t* thisp, cube_cartesian* cart, cube_thread* thrd, long int* coord) {
  if (!cart)
    return;
  cube_cart_def_coords(cart, thrd, coord);
}

void cube_set_sev(cube_t* thisp, cube_metric* met, cube_cnode* cnode, cube_thread* thrd, double value) {
  if (thisp->sev_flag == 1) {
    cube_sev_init(thisp);
    thisp->sev_flag = 0;
  }
#if DEBUG
  printf("metric_id=%d cnode_id=%d thread_id=%d exist=%d cn_exist=%d ",
        cube_metric_get_id(met), cube_cnode_get_id(cnode), cube_thread_get_id(thrd),
        thisp->exist[cube_metric_get_id(met)][cube_cnode_get_id(cnode)],
        thisp->cn_exist[cube_metric_get_id(met)][cube_cnode_get_id(cnode)][cube_thread_get_id(thrd)]);
  if (thisp->sev[cube_metric_get_id(met)][cube_cnode_get_id(cnode)])
      printf("value=%g", thisp->sev[cube_metric_get_id(met)][cube_cnode_get_id(cnode)][cube_thread_get_id(thrd)]);
  else printf("NOVALUE");
  printf(" -> %g\n", value);
#endif
 
  /* if the pointer doesn't exist, create it and store the value */
  if (thisp->exist[cube_metric_get_id(met)][cube_cnode_get_id(cnode)] == 0) {
    thisp->sev[cube_metric_get_id(met)][cube_cnode_get_id(cnode)] = (double *)calloc(thisp->thrd_ar->size, sizeof(double));
    if (thisp->sev[cube_metric_get_id(met)][cube_cnode_get_id(cnode)] == NULL) { printf("Malloc error\n"); exit(1); }
    thisp->exist[cube_metric_get_id(met)][cube_cnode_get_id(cnode)] = 1;
    thisp->sev[cube_metric_get_id(met)][cube_cnode_get_id(cnode)][cube_thread_get_id(thrd)] = value;
    thisp->cn_exist[cube_metric_get_id(met)][cube_cnode_get_id(cnode)][cube_thread_get_id(thrd)] = 1;
  }
  /* if it already contains a value, add the new value to it */
  else if (thisp->exist[cube_metric_get_id(met)][cube_cnode_get_id(cnode)] == 1 && 
           thisp->cn_exist[cube_metric_get_id(met)][cube_cnode_get_id(cnode)][cube_thread_get_id(thrd)] == 1) {
    thisp->sev[cube_metric_get_id(met)][cube_cnode_get_id(cnode)][cube_thread_get_id(thrd)] += value;
  }
  /* if the pointer exists, but contains no value, store the value */
  else {
    thisp->sev[cube_metric_get_id(met)][cube_cnode_get_id(cnode)][cube_thread_get_id(thrd)] = value;
    thisp->cn_exist[cube_metric_get_id(met)][cube_cnode_get_id(cnode)][cube_thread_get_id(thrd)] = 1;
  }
}

void cube_set_sev_reg(cube_t* thisp, cube_metric* met, cube_region* region, cube_thread* thrd, double value) {
  int i = 0;
  cube_cnode* v_cnode = NULL;
  if (thisp->sev_flag == 1) {
    cube_sev_init(thisp);
    thisp->sev_flag = 0;
  }
  for (i = 0; i < thisp->cnd_ar->size; i++) {
    cube_region* tmp = cube_cnode_get_callee((cube_cnode*)(thisp->cnd_ar->data[i]));
    if (cube_region_equal(tmp, region) == 1) {
      v_cnode = (cube_cnode*)(thisp->cnd_ar->data[i]);
      break;
    }
  }
  if (v_cnode == NULL) {
    v_cnode = cube_def_cnode_cs(thisp, region, cube_region_get_mod(region), -1, NULL);
  }
  cube_set_sev(thisp, met, v_cnode, thrd, value);
}

void cube_add_sev(cube_t* thisp, cube_metric* met, cube_cnode* cnode, cube_thread* thrd, double incr) {
  double val;
  if (thisp->sev_flag == 1) {
    cube_sev_init(thisp);
    thisp->sev_flag = 0;
  }
  val = cube_get_sev(thisp, met, cnode, thrd);
  cube_set_sev(thisp, met, cnode, thrd, val+incr);
}

void cube_add_sev_reg(cube_t* thisp, cube_metric* met, cube_region* region, cube_thread* thrd, double incr) {
  cube_cnode* cn;
  int i = 0;
  double val;
  if (thisp->sev_flag == 1) {
    cube_sev_init(thisp);
    thisp->sev_flag = 0;
  }
  for (i = 0; i < thisp->cnd_ar->size; i++) {
    cube_region* tmp = cube_cnode_get_callee((cube_cnode*)(thisp->cnd_ar->data[i]));
    if (cube_region_equal(tmp, region) == 1) {
      cn = (cube_cnode*)(thisp->cnd_ar->data[i]);
      break;
    }
  }
  if (i == thisp->cnd_ar->size) printf("cube_add_sev_reg(): region undefined\n");
  val = cube_get_sev(thisp, met, cn, thrd);
  cube_set_sev(thisp, met, cn, thrd, val+incr);
}

void cube_assign_ids(cube_t* thisp) {
  int id = 0;
  int i = 0;

  /* metrics */
  for (i = 0; i < thisp->rmet_ar->size; i++) {
    cube_metric_assign_ids((cube_metric*)(thisp->rmet_ar->data[i]), &id);
  }
      
  /* regions */
  for (i = 0; i < thisp->reg_ar->size; i++)
    cube_region_set_id((cube_region*)(thisp->reg_ar->data[i]), i);
      
  /* cnodes */
  id = 0;
  for (i = 0; i < thisp->rcnd_ar->size; i++) {
    cube_cnode_assign_ids((cube_cnode*)(thisp->rcnd_ar->data[i]), &id);
  }

  /* machines */
  for (i = 0; i < thisp->mach_ar->size; i++)
    cube_machine_set_id((cube_machine*)(thisp->mach_ar->data[i]), i);
 
  /* nodes */
  for (i = 0; i < thisp->node_ar->size; i++)
    cube_node_set_id((cube_node*)(thisp->node_ar->data[i]), i);
  
  /* processes */
  for (i = 0; i < thisp->proc_ar->size; i++)
    cube_process_set_id((cube_process*)(thisp->proc_ar->data[i]), i);

  /* threads */
  for (i = 0; i < thisp->thrd_ar->size; i++)
    cube_thread_set_id((cube_thread*)(thisp->thrd_ar->data[i]), i);
}
  
dyn_array* cube_get_rmet(cube_t* thisp) {
  return thisp->rmet_ar;
}

dyn_array* cube_get_met(cube_t* thisp) {
  return thisp->met_ar;
}

dyn_array* cube_get_rcnd(cube_t* thisp) {
  return thisp->rcnd_ar;
}

dyn_array* cube_get_cnd(cube_t* thisp) {
  return thisp->cnd_ar;
}

dyn_array* cube_get_mirr(cube_t* thisp) {
  return thisp->mirr_ar;
}

dyn_array* cube_get_reg(cube_t* thisp) {
  return thisp->reg_ar;
}

dyn_array* cube_get_mach(cube_t* thisp) {
  return thisp->mach_ar;
}

dyn_array* cube_get_thrd(cube_t* thisp) {
  return thisp->thrd_ar;
}

dyn_array* cube_get_cart(cube_t* thisp) {
  return thisp->cart_ar;
}

att_array* cube_get_attr(cube_t* thisp) {
  return thisp->attr;
}

double cube_get_sev(cube_t* thisp, cube_metric* met, cube_cnode*  cnode, cube_thread* thrd) {
  if (thisp->exist[cube_metric_get_id(met)][cube_cnode_get_id(cnode)] == 1) {
    if (thisp->cn_exist[cube_metric_get_id(met)][cube_cnode_get_id(cnode)][cube_thread_get_id(thrd)] == 1) {
      return thisp->sev[cube_metric_get_id(met)][cube_cnode_get_id(cnode)][cube_thread_get_id(thrd)];
    }
    else return 0; 
  }
  return 0;
}

void cube_write_all(cube_t* thisp, FILE* fp) {
  cube_write_def(thisp, fp);
  cube_write_sev_matrix(thisp, fp);
}

void cube_write_def(cube_t* thisp, FILE* fp) {
  dyn_array* rmet = cube_get_rmet(thisp);
  dyn_array* rcnd = cube_get_rcnd(thisp);
  dyn_array* reg  = cube_get_reg(thisp);
  dyn_array* mach = cube_get_mach(thisp);
  dyn_array* cart = cube_get_cart(thisp);
  dyn_array* mirr = cube_get_mirr(thisp);
  att_array* attr = cube_get_attr(thisp);

  int i = 0;

  cube_assign_ids(thisp);

  /* xml header */
  fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n");
  fprintf(fp, "<cube version=\"3.0\">\n");

  /* metadata info */
  for (i = 0; i < attr->size; i++) {
    char* key = attr->data[i]->key;
    char* value = attr->data[i]->value;
    fprintf(fp, "  <attr key=\"%s\" value=\"%s\"/>\n", key, value);
  } 

  /* mirrored URLs */
  fprintf(fp, "  <doc>\n");
  fprintf(fp, "    <mirrors>\n");
  for (i = 0; i < mirr->size; i++) {
    char* mirror = (char *) mirr->data[i];
    fprintf(fp, "      <murl>%s</murl>\n", mirror);
  }
  fprintf(fp, "    </mirrors>\n"); 
  fprintf(fp, "  </doc>\n");

  /* metrics */
  fprintf(fp, "  <metrics>\n");
  for (i = 0; i < rmet->size; i++) {  /* write metrics */
    cube_metric* m = (cube_metric *) rmet->data[i];
    cube_metric_writeXML(m, fp);  
  }
  fprintf(fp, "  </metrics>\n");

  /* program */
  fprintf(fp, "  <program>\n");

  for (i = 0; i < reg->size; i++) {    /* write regions */
    cube_region* r = (cube_region *) reg->data[i];
    cube_region_writeXML(r, fp);
  }

  for (i = 0; i < rcnd->size; i++) {   /* write cnodes */
    cube_cnode* c = (cube_cnode*) rcnd->data[i];
    cube_cnode_writeXML(c, fp);
  }

  fprintf(fp, "  </program>\n");

  /* system */
  fprintf(fp, "  <system>\n");
  for (i = 0; i < mach->size; i++) {    /* write system resources */
    cube_machine* m = (cube_machine *) mach->data[i];
    cube_machine_writeXML(m, fp);
  }

  /* topologies */
  fprintf(fp, "    <topologies>\n");
  for (i = 0; i < cart->size; i++) { /* write topologies */
    cube_cartesian *c = (cube_cartesian *) cart->data[i];
    cube_cart_writeXML(c, fp);      
  }
  fprintf(fp, "    </topologies>\n");
  fprintf(fp, "  </system>\n"); 
}

void cube_write_sev_matrix(cube_t* thisp, FILE* fp) {
  int i = 0, j = 0, k = 0, p = 0;
  int all_zero = 1;

  dyn_array* met  = cube_get_met(thisp);
  dyn_array* cnd  = cube_get_cnd(thisp);
  dyn_array* thrd = cube_get_thrd(thisp);

  /* severity */
  fprintf(fp, "  <severity>\n");

  for (i = 0; i < met->size; i++) {
    cube_metric* m = (cube_metric *) met->data[i];
    fprintf(fp, "    <matrix metricId=\"%d\">\n", cube_metric_get_id(m));
    for (j = 0; j < cnd->size; j++) {
      cube_cnode* c = (cube_cnode *) cnd->data[j];
      int ztest = 0;
      for (p = 0; p < thrd->size; p++) {
        cube_thread* t = (cube_thread *) thrd->data[p];
        if (cube_get_sev(thisp, m, c, t) != 0.0) { all_zero = 0; break; }
        else all_zero = 1;
      }
      if (all_zero == 0) {
        for (k = 0; k < thrd->size; k++) {
          cube_thread* t = (cube_thread *) thrd->data[k];
          /* if (cube_get_sev(thisp, m, c, t) != 0)  { */
            if (ztest == 0) {
              ztest = 1;
              fprintf(fp, "      <row cnodeId=\"%d\">\n", cube_cnode_get_id(c));
            }
            /* fprintf(fp, "        <val thrdId=\"%d\">", cube_thread_get_id(t)); */
            /* fprintf(fp, "%.12g</val>\n", cube_get_sev(thisp, m, c, t)); */
            fprintf(fp, "%.12g\n", cube_get_sev(thisp, m, c, t));
          /* } */
        }
        if (ztest)
          fprintf(fp, "      </row>\n"); 
      }
    }
    fprintf(fp, "    </matrix>\n"); 
  }

  fprintf(fp, "  </severity>\n"); 
  fprintf(fp, "</cube>\n");
  thisp->sev_flag = -1;
}

void cube_write_sev_row(cube_t* thisp, FILE* fp, cube_metric* met, cube_cnode* cnd, double* sevs) {
  static int first = 1;
  static int curr = -1;
  int i = 0, p = 0;
  int ztest = 0, all_zero = 1;

  if (first) {
    fprintf(fp, "  <severity>\n");
    first = 0;
  }

  if (cube_metric_get_id(met) != curr) {
    if (curr != -1) fprintf(fp, "    </matrix>\n");
    if (strcmp(cube_metric_get_val(met), "VOID") == 0) return;
    fprintf(fp, "    <matrix metricId=\"%d\">\n", cube_metric_get_id(met));
    curr = cube_metric_get_id(met);
  }

  /* XXXX WARNING XXXX
     The following code uses each thread's parent process rank to index into
     the vector of severity values (sevs).  By tracking the previously used
     process rank (last_prank), it avoids replicating the values for threads
     that have the same process (at least when reported in process order).
     When real thread severities are provided (instead of process severities)
     thisp code will need to be changed.
  */

  all_zero = 1;
  for (p = 0; p < thisp->thrd_ar->size; p++) {
    if (sevs[p] != 0.0) {
      all_zero = 0;
      break;
    }
  }

  if (all_zero == 0 && thisp->thrd_ar->size > 0) {
    int last_prank = -1;

    /* Write severity data */
    fprintf(fp, "      <row cnodeId=\"%d\">\n", cube_cnode_get_id(cnd));
    for (i = 0; i < thisp->thrd_ar->size; i++) {
      int prank = cube_process_get_rank(cube_thread_get_parent((cube_thread*)thisp->thrd_ar->data[i]));
        if (prank == last_prank)
          fprintf(fp, "0\n");
        else
          fprintf(fp, "%.12g\n", sevs[prank]);
        last_prank = prank;
    }
    fprintf(fp, "      </row>\n"); 
  }
}

void cube_write_finish(cube_t* thisp, FILE* fp) {
  if (thisp->sev_flag != -1) {
    fprintf(fp, "    </matrix>\n"); 
    fprintf(fp, "  </severity>\n"); 
  }
  fprintf(fp, "</cube>\n"); 
}
