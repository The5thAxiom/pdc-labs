      program testA

      !$OMP BARRIER

      !$POMP NOINSTRUMENT

      !$POMP INST &
      !$POMP BEGIN(foo)

      !$OMP PARALLEL
      i = 9
      !$OMP end PARALLEL

      !$POMP INST END(foo)

      !$POMP INSTRUMENT

      !$OMP BARRIER

      end
