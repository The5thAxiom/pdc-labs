int main() {
  int i;

#pragma \
 /*--*/ pomp \
 /*--*/ inst \
 /*--*/ init

#pragma pomp inst off

#pragma pomp inst begin(foo)

#pragma omp parallel
      i = 9;

#pragma pomp inst end(foo)

#pragma pomp inst on

#pragma pomp inst finalize

}
