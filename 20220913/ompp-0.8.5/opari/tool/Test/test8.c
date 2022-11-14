int main() {
  int i;

#pragma omp inst init

#pragma omp inst off

#pragma omp inst begin(foo)

#pragma omp parallel
      i = 9;

#pragma omp inst end(foo)

#pragma omp inst on

#pragma omp inst finalize

}
