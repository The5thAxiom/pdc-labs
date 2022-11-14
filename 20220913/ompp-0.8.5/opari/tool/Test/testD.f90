subroutine foo(b)
  logical b
  integer i

  !$POMP INST BEGIN(foo)
  i = 4
  if ( b ) then
    !$POMP INST ALTEND(foo)
    return
  end if

  !$OMP PARALLEL

    !$OMP INST BEGIN(phase1)

    !$OMP BARRIER

    !$OMP INST END(phase1)

  !$OMP END PARALLEL

  if ( b ) then
    !$POMP INST ALTEND(foo)
    return
  end if

  !$POMP INST END(foo)
end subroutine foo
