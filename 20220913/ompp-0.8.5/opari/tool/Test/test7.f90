      program test7
      integer, dimension(10,10) :: AA, BB, CC

      !$omp parallel
      !$omp workshare
        AA = BB
        BB = CC
      !$omp end workshare
      !$omp end parallel

      !$omp parallel
      !$omp workshare
        AA = BB
        BB = CC
      !$omp end workshare nowait
      !$omp end parallel

      !$omp parallel workshare
        AA = BB
        BB = CC
      !$omp end parallel workshare

      end
