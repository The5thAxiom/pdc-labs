      program test3

c$OMP    parallel
c$OMP*   do
      do i=1,4
        write(*,*) "pdo", i
      enddo
!$omp end
!$omp+
!$omp+parallel
!$omp+
!$omp+do
      end
