       program testG
         integer me, glob
         integer omp_get_thread_num

         !$omp parallel private(me,glob)
           me = omp_get_thread_num()
           write(*,*) "hello from", me

           !$omp single
             glob = me
             write(*,*) "single1 from", me
           !$omp end single copyprivate(glob)

           write(*,*) "hello from", me

           !$omp single 
             write(*,*) "single2 from", me
           !$omp end single 

           !$omp single
             write(*,*) "single3 from", me
           !$omp end single nowait

         !$omp end parallel
       end program testG
