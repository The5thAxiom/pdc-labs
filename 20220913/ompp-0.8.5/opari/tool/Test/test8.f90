      program test8

C$OMP INST INIT

      !$OMP INST OFF

      !$OMP INST BEGIN(foo)

      !$OMP PARALLEL
      i = 9
      !$OMP end PARALLEL

      !$OMP INST END(foo)

      !$OMP INST ON

   !$OMP INSTFINALIZE

      end
