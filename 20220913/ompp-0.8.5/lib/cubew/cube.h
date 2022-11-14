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

#ifndef CUBEW_CUBE_H
#define CUBEW_CUBE_H

#ifdef __cplusplus
extern "C" {
#endif

  #include  "cartesian.h"
  #include  "cnode.h"
  #include  "machine.h"
  #include  "metric.h"
  #include  "node.h"
  #include  "process.h"
  #include  "region.h"
  #include  "thread.h"

  typedef struct dyn_array cube_dyn_array;
  typedef struct cmap cube_map;
  typedef struct att_array cube_att_array; 

  typedef struct cube_t {
    cube_dyn_array* met_ar;
    cube_dyn_array* rmet_ar;
    cube_dyn_array* reg_ar;
    cube_dyn_array* cnd_ar;
    cube_dyn_array* rcnd_ar;
    cube_dyn_array* mach_ar;
    cube_dyn_array* node_ar;
    cube_dyn_array* proc_ar;
    cube_dyn_array* thrd_ar;
    cube_dyn_array* cart_ar;
    cube_dyn_array* mirr_ar;
    cube_att_array* attr;
    int sev_flag;
    double*** sev;
    int** exist;
    int*** cn_exist;
    char* filename;
  } cube_t;


  cube_t*         cube_create();
  void            cube_sev_init(cube_t* cb);
  void            cube_free(cube_t* cb);
  void            cube_construct_arrays(cube_t* cb);
  void            cube_add_metric(cube_t* cb, cube_metric* met);
  void            cube_add_rmetric(cube_t* cb, cube_metric* met);
  void            cube_add_region(cube_t* cb, cube_region* reg);
  void            cube_add_cnode(cube_t* cb, cube_cnode* cnd);
  void            cube_add_rcnode(cube_t* cb, cube_cnode* cnd);
  void            cube_add_machine(cube_t* cb, cube_machine* mach);
  void            cube_add_node(cube_t* cb, cube_node* node);
  void            cube_add_process(cube_t* cb, cube_process* proc);
  void            cube_add_thread(cube_t* cb, cube_thread* thrd);
  void            cube_add_cart(cube_t* cb, cube_cartesian* cart);
  void            cube_add_mirror(cube_t* cb, const char* mir);
  void            cube_add_attr(cube_t* cb, cube_map* m); 

  void            cube_def_attr(cube_t* cb, const char* key, const char* value);
  void            cube_def_mirror(cube_t* cb, const char* url);
  cube_metric*    cube_def_met(cube_t* cb, const char* disp_name, const char* uniq_name, const char* dtype,
                          const char* uom, const char* val, const char* url, const char* descr, cube_metric* parent);
  cube_region*    cube_def_region(cube_t* cb, const char* name, long begln, long endln, 
         	             const char* url, const char* descr, const char* mod);
  cube_cnode*     cube_def_cnode_cs(cube_t* cb, cube_region* callee, const char* mod, int line, cube_cnode* parent);
  cube_cnode*     cube_def_cnode(cube_t* cb, cube_region* callee, cube_cnode* parent);
  cube_machine*   cube_def_mach(cube_t* cb, const char* name,  const char* desc);
  cube_node*      cube_def_node(cube_t* cb, const char* name, cube_machine* mach);
  cube_process*   cube_def_proc(cube_t* cb, const char* name, int rank, cube_node* node);
  cube_thread*    cube_def_thrd(cube_t* cb, const char* name, int rank, cube_process* proc);
  cube_cartesian* cube_def_cart(cube_t* cb, long int ndims, long int* dim, int* period);
  void            cube_def_coords(cube_t* cb, cube_cartesian* cart, cube_thread* thrd, long int* coord);

  void            cube_set_sev(cube_t* cb, cube_metric* met, cube_cnode* cnode, cube_thread* thrd, double value);
  void            cube_set_sev_reg(cube_t* cb, cube_metric* met, cube_region* reg, cube_thread* thrd, double value);
  void            cube_add_sev(cube_t* cb, cube_metric* met, cube_cnode* cnode, cube_thread* thrd, double incr);
  void            cube_add_sev_reg(cube_t* cb, cube_metric* met, cube_region* reg, cube_thread* thrd, double incr);

  cube_dyn_array* cube_get_rmet(cube_t* cb);
  cube_dyn_array* cube_get_met(cube_t* cb);
  cube_dyn_array* cube_get_rcnd(cube_t* cb);
  cube_dyn_array* cube_get_cnd(cube_t* cb);
  cube_dyn_array* cube_get_reg(cube_t* cb);
  cube_dyn_array* cube_get_mach(cube_t* cb);
  cube_dyn_array* cube_get_thrd(cube_t* cb);
  cube_dyn_array* cube_get_cart(cube_t* cb);
  cube_dyn_array* cube_get_mirr(cube_t* cb);
  cube_att_array* cube_get_attr(cube_t* cb);
  double          cube_get_sev(cube_t* cb, cube_metric* met, cube_cnode* cnode, cube_thread* thrd);

  void            cube_assign_ids(cube_t* cb);

  void            cube_write_all(cube_t* cb, FILE* fp);
  void            cube_write_def(cube_t* cb, FILE* fp);
  void            cube_write_sev_matrix(cube_t* cb, FILE* fp);
  void            cube_write_sev_row(cube_t* cb, FILE* fp, cube_metric* met, cube_cnode* cnd, double* sevs);
  void            cube_write_finish(cube_t* cb, FILE* fp);
 
#ifdef __cplusplus
}
#endif

#endif

  




