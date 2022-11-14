#include <omp.h>

int main() {
  // omp_init_lock(i)
  /* -- omp_init_lock(i) -- */
  "omp_init_lock(i)"  // omp_init_lock(i)
  "omp_init_lock(i)\"test" "\"omp_init_lock(i)" "omp_init_lock(i)\""
  """" "\"""\""

  omp_init_lock(i);       // omp_init_lock(i)
  omp_init_lock(i); omp_set_lock(i); omp_destroy_lock(i);
  printf("omp_init_lock(i)"); omp_init_lock(i);
  omp_init_locks(i);
}
