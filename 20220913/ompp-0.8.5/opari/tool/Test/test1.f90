      program test1
      gg = 9
       !$Omp parallel
       !  !$Omp no
      gg = 7
       !$OMP end          &
       !$OMP parallel
       !$OMP parallel     &
       !$OMP &  default   &
       !$OMP &  (shared)
       !$OMP end          &     !extra comment
       !$OMP parallel
      end
