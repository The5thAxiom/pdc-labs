void foo (bool b) {
  int i;

  #pragma pomp inst begin(foo)
  i = 4;
  if (b) {
    #pragma pomp inst altend(foo)
    return;
  }

  #pragma omp parallel
  {
    #pragma omp inst begin(phase1)

    #pragma omp barrier

    #pragma omp inst end(phase1)
  }

  if (b) {
    #pragma pomp inst altend(foo)
    return;
  }

  #pragma pomp inst end(foo)
}
