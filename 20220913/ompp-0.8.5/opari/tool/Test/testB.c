int main() {
  int i;

  #pragma omp parallel
  {
    #pragma omp for
    for (i=0; i<4; ++i)
    {
    }
  }

  #pragma omp parallel
    #pragma omp for
    for (i=0; i<4; ++i)
    {
    }

  #pragma omp parallel
  {
    #pragma omp for
    for (i=0; i<4; ++i)
      ;
  }

  #pragma omp parallel
  {
    #pragma omp for
    for (i=0; i<4; ++i) ;
  }

  #pragma omp parallel
    #pragma omp for
    for (i=0; i<4; ++i) ;
}
