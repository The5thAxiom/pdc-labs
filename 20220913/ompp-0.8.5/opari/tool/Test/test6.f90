      program test6

      ! call omp_init_lock(i)
C f77 call omp_init_lock(i)
      "omp_init_lock(i)"  'omp_init_lock(i)'    ! call omp_init_lock(i)
      "omp_init_lock(i)""test" """omp_init_lock(i)" "omp_init_lock(i)"""
      """" """"""""

      "omp_init_lock(i)
C        "
     +omp_init_lock(i)"

      "omp_init_lock(i)    &
     !   "
      &omp_init_lock(i)"  call omp_init_lock(i)

      call omp_init_lock(i)       ! call omp_init_lock(i)
      call omp_init_lock(i) ; call omp_set_lock(i) ; call omp_destroy_lock(i)
      write(*,*) "call omp_init_lock(i)" ; call omp_init_lock(i)

C$C99 call omp_init_lock(i)
    !$DOMP  call omp_init_lock(i)

*$    call omp_init_lock(i)
*P$   call omp_init_lock(i)
    !$  call omp_init_lock(i)
    !P$ call omp_init_lock(i)

      end
