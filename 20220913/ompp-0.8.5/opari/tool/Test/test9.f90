      program test9

C$POMP  INST
C$POMP& INIT

      !$POMP INST OFF

      !$POMP INST &
      !$POMP BEGIN(foo)

      !$OMP PARALLEL
      i = 9
      !$OMP end PARALLEL

      !$POMP INST END(foo)

      !$POMP INST ON

   !$POMP INSTFINALIZE

      end
