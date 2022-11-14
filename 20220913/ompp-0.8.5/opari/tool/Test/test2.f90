       program test2
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
           write(*,*) "do",i
         enddo
         !$omp end do

         !$omp sections
           !$omp section
           write(*,*) "section nowait 1"
           !$omp section
           write(*,*) "section nowait 2"
         !$omp end sections nowait

         !$omp master
         write(*,*) "master"
         !$omp end master

         !$omp critical
         write(*,*) "critical"
         !$omp end critical

         !$omp critical(foobar)
         write(*,*) "critical"
         !$omp end critical(foobar)

         !$omp atomic
         ! do this atomic
         i = i + 1

         !$omp single
         write(*,*) "single"
         !$omp end single

         !$omp sections
           !$omp section
           write(*,*) "section 1"
           !$omp section
           write(*,*) "section 2"
         !$omp endsections

       !$omp end parallel

       write(*,*) "sequential1"

       !$omp parallel do
       do i=1,4
         write(*,*) "pdo",i
       enddo
       !$omp end parallel do

       write(*,*) "sequential2"

       !$omp parallelsections
         !$omp section
         write(*,*) "psection 1"
         !$omp section
         write(*,*) "psection 2"
       !$omp end parallelsections

       end
