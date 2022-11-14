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

#ifndef CUBEW_CARTESIAN_H
#define CUBEW_CARTESIAN_H   

#ifdef __cplusplus
extern "C" {
#endif

  #include <stdio.h>
  #include "machine.h"
  #include "node.h"
  #include "process.h"
  #include "thread.h"

  typedef struct d_arr cube_d_arr;
  typedef struct p_arr cube_p_arr;
  typedef struct map_thrd cube_map_thrd; 
  typedef struct array cube_array;

  typedef struct cube_cartesian {
    long int ndims;
    cube_d_arr* dim;
    cube_p_arr* period;
    cube_array* thrd2coord;
  } cube_cartesian;

  cube_cartesian* cube_cart_create(cube_cartesian* cart);
  void cube_cart_init(cube_cartesian* cart, long int ndims, long int* dim, int* period);
  void cube_cart_construct_arrays(cube_cartesian* cart);
  void cube_cart_add_dim(cube_cartesian* cart, long int dim);
  void cube_cart_add_per(cube_cartesian* cart, int period);
  void cube_cart_add_thrd2coord(cube_cartesian* cart, cube_map_thrd *m);
  void cube_cart_free(cube_cartesian* cart);
  void cube_cart_def_coords(cube_cartesian* cart, cube_thread* thrd, long int* coord); 
  void cube_cart_writeXML(cube_cartesian* cart, FILE* fp);

#ifdef __cplusplus
}
#endif

#endif
