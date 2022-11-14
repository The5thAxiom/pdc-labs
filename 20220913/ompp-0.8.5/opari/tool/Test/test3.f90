      program test3

      !$omp     parallel     & !test !test !test
            !$omp           sections
            write(*,*) "psection"
      !$omp end              &
            !$omp parallel   &
            !$omp sections

      !$omp parallel sections
            write(*,*) "psection"
      !$omp end              &
            !$omp&parallel   &
            !$omp & sections
      end
