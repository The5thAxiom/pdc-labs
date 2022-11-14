      program test1
      gg = 9
c     comment
c$OMP parallel
*$oMp barrier
!$OmP barrier
c $omp no
!!$omp mo
!$OmP end
!$OmP+
!$OmP+parallel
!$OmP atomic
      gg = 7
!$OmP    parallel
!$OmP&   default(shared)
      gg = 7
!$OmP    end
!$OmP+   parallel
      end
