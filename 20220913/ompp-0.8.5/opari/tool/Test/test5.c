#include <stdio.h>

int main() {
  int i, j, k, l;
  #pragma omp parallel         /* parallel */ \
      for                      /* do */ \
      lastprivate(k)           /* do */ \
      private(i,j)             /* parallel */ \
      lastprivate              /* do */ \
      (                        /* do */ \
        l                      /* do */ \
      ) schedule(dynamic       /* do */ \
      )
  for(i=0; i<4;++i) {
    printf("pdo %d\n", i);
  }
}
