      program test5
      integer i, j, k, l
      !$omp parallel                          & !parallel
      !$omp & do                              & !do
      !$omp & lastprivate(k)                  & !do
      !$omp & private(i,j)                    & !parallel
      !$omp & lastprivate                     & !do
      !$omp & (                               & !do
      !$omp &   l                             & !do
      !$omp & ) schedule(dynamic              & !do
      !$omp   )
      do i=1,4
        write(*,*) "pdo", i
      enddo
      !$omp end parallel do
      end
