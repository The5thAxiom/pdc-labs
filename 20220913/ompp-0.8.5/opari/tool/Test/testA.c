int main() {
  int i;

#pragma omp barrier

#pragma pomp no instrument

#pragma pomp inst begin(foo)

#pragma omp parallel
      i = 9;

#pragma pomp inst end(foo)

#pragma pomp instrument

#pragma omp barrier
}
