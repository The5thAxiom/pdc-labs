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
 * Fortran wrappers calling the C versions
 */

void FSUB(POMP_Atomic_enter)(int* id) {
  if ( pomp_tracing ) POMP_Atomic_enter(pomp_rd_table[*id]);
}

void FSUB(POMP_Atomic_exit)(int* id) {
  if ( pomp_tracing ) POMP_Atomic_exit(pomp_rd_table[*id]);
}

void FSUB(POMP_Barrier_enter)(int* id) {
  if ( pomp_tracing ) POMP_Barrier_enter(pomp_rd_table[*id]);
}

void FSUB(POMP_Barrier_exit)(int* id) {
  if ( pomp_tracing ) POMP_Barrier_exit(pomp_rd_table[*id]);
}

void FSUB(POMP_Flush_enter)(int* id) {
  if ( pomp_tracing ) POMP_Flush_enter(pomp_rd_table[*id]);
}

void FSUB(POMP_Flush_exit)(int* id) {
  if ( pomp_tracing ) POMP_Flush_exit(pomp_rd_table[*id]);
}

void FSUB(POMP_Critical_begin)(int* id) {
  if ( pomp_tracing ) POMP_Critical_begin(pomp_rd_table[*id]);
}

void FSUB(POMP_Critical_end)(int* id) {
  if ( pomp_tracing ) POMP_Critical_end(pomp_rd_table[*id]);
}

void FSUB(POMP_Critical_enter)(int* id) {
  if ( pomp_tracing ) POMP_Critical_enter(pomp_rd_table[*id]);
}

void FSUB(POMP_Critical_exit)(int* id) {
  if ( pomp_tracing ) POMP_Critical_exit(pomp_rd_table[*id]);
}

void FSUB(POMP_Do_enter)(int* id) {
  if ( pomp_tracing ) POMP_For_enter(pomp_rd_table[*id]);
}

void FSUB(POMP_Do_exit)(int* id) {
  if ( pomp_tracing ) POMP_For_exit(pomp_rd_table[*id]);
}

void FSUB(POMP_Master_begin)(int* id) {
  if ( pomp_tracing ) POMP_Master_begin(pomp_rd_table[*id]);
}

void FSUB(POMP_Master_end)(int* id) {
  if ( pomp_tracing ) POMP_Master_end(pomp_rd_table[*id]);
}

void FSUB(POMP_Parallel_begin)(int* id) {
  if ( pomp_tracing ) POMP_Parallel_begin(pomp_rd_table[*id]);
}

void FSUB(POMP_Parallel_end)(int* id) {
  if ( pomp_tracing ) POMP_Parallel_end(pomp_rd_table[*id]);
}

void FSUB(POMP_Parallel_fork)(int* id) {
  if ( pomp_tracing ) POMP_Parallel_fork(pomp_rd_table[*id]);
}

void FSUB(POMP_Parallel_join)(int* id) {
  if ( pomp_tracing ) POMP_Parallel_join(pomp_rd_table[*id]);
}

void FSUB(POMP_Section_begin)(int* id) {
  if ( pomp_tracing ) POMP_Section_begin(pomp_rd_table[*id]);
}

void FSUB(POMP_Section_end)(int* id) {
  if ( pomp_tracing ) POMP_Section_end(pomp_rd_table[*id]);
}

void FSUB(POMP_Sections_enter)(int* id) {
  if ( pomp_tracing ) POMP_Sections_enter(pomp_rd_table[*id]);
}

void FSUB(POMP_Sections_exit)(int* id) {
  if ( pomp_tracing ) POMP_Sections_exit(pomp_rd_table[*id]);
}

void FSUB(POMP_Single_begin)(int* id) {
  if ( pomp_tracing ) POMP_Single_begin(pomp_rd_table[*id]);
}

void FSUB(POMP_Single_end)(int* id) {
  if ( pomp_tracing ) POMP_Single_end(pomp_rd_table[*id]);
}

void FSUB(POMP_Single_enter)(int* id) {
  if ( pomp_tracing ) POMP_Single_enter(pomp_rd_table[*id]);
}

void FSUB(POMP_Single_exit)(int* id) {
  if ( pomp_tracing ) POMP_Single_exit(pomp_rd_table[*id]);
}

void FSUB(POMP_Workshare_enter)(int* id) {
  if ( pomp_tracing ) POMP_Workshare_enter(pomp_rd_table[*id]);
}

void FSUB(POMP_Workshare_exit)(int* id) {
  if ( pomp_tracing ) POMP_Workshare_exit(pomp_rd_table[*id]);
}
