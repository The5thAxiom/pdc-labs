      program test
      integer i,j,k,l

!$omp  parallel
!$omp+ sections
!$omp+ default(shared) private(i) lastprivate(k)
!$omp+ lastprivate(l)
!$omp  section
         write(*,*) "section1"
!$omp  section
         write(*,*) "section2"
!$omp  end parallel sections

      end
