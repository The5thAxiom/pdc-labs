subroutine foo(b)
  logical b
  integer i

  !POMP$ INST BEGIN(foo)
  i = 4
  if ( b ) then
    !POMP$ INST ALTEND(foo)
    return
  end if

  !$OMP PARALLEL

CPOMP$ INST BEGIN(phase1)
*POMP$ INST BEGIN(phase2)

    !$OMP BARRIER

*POMP$ INST END(phase2)
CPOMP$ INST END(phase1)

  !$OMP END PARALLEL

  if ( b ) then
    !POMP$ INST ALTEND(foo)
    return
  end if

  !POMP$ INST END(foo)
end subroutine foo
