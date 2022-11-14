/****************************************************************************
**  SCALASCA    http://www.scalasca.org/                                   **
**  KOJAK       http://www.fz-juelich.de/jsc/kojak/                        **
*****************************************************************************
**  Copyright (c) 1998-2009                                                **
**  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
**                                                                         **
**  See the file COPYRIGHT in the package base directory for details       **
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "pomp_lib.h"
#include "pomp_fwrapper_def.h"

/*
 * Basic Fortran wrappers calling the C versions
 */

void FSUB(POMP_Finalize)() {
  POMP_Finalize();
}

void FSUB(POMP_Init)() {
  POMP_Init();
}

void FSUB(POMP_Off)() {
  pomp_tracing = 0;
}

void FSUB(POMP_On)() {
  pomp_tracing = 1;
}

void FSUB(POMP_Begin)(int* id) {
  if ( pomp_tracing ) POMP_Begin(pomp_rd_table[*id]);
}

void FSUB(POMP_End)(int* id) {
  if ( pomp_tracing ) POMP_End(pomp_rd_table[*id]);
}
