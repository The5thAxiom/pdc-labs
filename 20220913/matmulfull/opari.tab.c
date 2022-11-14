#include "pomp_lib.h"


extern struct ompregdescr omp_rd_7;
extern struct ompregdescr omp_rd_8;

int POMP_MAX_ID = 9;

struct ompregdescr* pomp_rd_table[9] = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  &omp_rd_7,
  &omp_rd_8,
};
