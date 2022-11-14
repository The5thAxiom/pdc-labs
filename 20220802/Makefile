case1:
	gcc case1.c -fopenmp -o case1.out && \
	./case1.out
case2:
	gcc case2.c -fopenmp -o case2.out && \
	./case2.out
case3:
	export OMP_NUM_THREADS=100 && \
	gcc case3.c -fopenmp -o case3.out && \
	./case3.out

vadds:
	gcc vadd_serial.c -fopenmp -o vadds.out && \
	./vadds.out

vaddp:
	gcc vadd_parallel.c -fopenmp -o vaddp.out
	./vaddp.out 4
	./vaddp.out 1000
	./vaddp.out 10000
	./vaddp.out 100000


vadd:
	make vadds vaddp
