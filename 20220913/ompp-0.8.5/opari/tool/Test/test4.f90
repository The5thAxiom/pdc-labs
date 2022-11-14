       program test4
       integer i

       !$omp parallel
       write(*,*) "parallel"
         !$omp do
         do i=1,4
           write(*,*) "do nowait",i
         enddo
         !$omp enddo nowait

         !$omp barrier

         !$omp do
         do i=1,4
           write(*,*) "do nowait?",i
         enddo
         !$omp   end   &  ! continue
         !$omp &       &  ! still continue
         !$omp & do       ! kein nowait

         !$omp do
         do i=1,4
           write(*,*) "do",i
         enddo
         !$omp end do
       !$omp end parallel

       end
