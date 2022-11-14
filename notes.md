<style>
    code {
        color: green;
    }
</style>

# 20220726

## Seeing Processor information

### `lscpu`

-   bash command:
    ```bash
      lscpu
    ```
-   Cache:
    -   L1i: for instructions
    -   L1d: for data

### hwloc

-   Install `hwloc` using apt/dnf/whatever: `sudo apt install hwloc`
-   Type `lstopo` in bash to open the GUI
-   NUMA: Non-Uniform Memory Access

## Multithreading

-   Different ways:
    -   pthreads
    -   Java threads
    -   openMP

### Processes

-   Processes are heavyweight, threads are lightweight
-   Seeing running proccesses:
    -   `ps -eaf` to see all running procces
    -   `cd /proc/PID` (get any PID) to go to the process's directory
    -   use `ls` to list all the process information

### pthreads

-   Only in POSIX compliant systems (not in Windows)
-   use `man pthread_create` to see the arguments needed to create a pthread (in C)
-   use `man pthreads` to see the man page
-   pthreads example:
    -   Copy the example code from the man page for `pthread_create`
    -   Compile it using `gcc filename -lpthread` and give the program some words as command line arguments: `./a.out hello how are you`. You will see the output as given in the manpage.

<hr/>

# 20220802

-   Why not to use pthreads:
    -   difficult to use
    -   only works on POSIX
-   so, we will use openMP
-   about OpenMP:
    -   a C-extension API
    -   stands for Open MultiProcessing
    -   for multithreaded, shared memory parallelism
    -   you can convert serial code into parallel code using openMP by including the openMP library: `#include<omp.h>`
    -   works mostly using preprocessor (or compiler?) directives:
        ```c
        #include <omp.h>
        #pragma omp parallel private
        ```
    -   by default, omp will create threads equal to the number of processors (4 on quad-core machines, 8 on octa-core, etc.)
    -   omp has around 35 directives, 44 runtime library routines and around 13 enviroment variables
    -   question:
        -   case 1: 1000 threads using runtime lib routines (what sir did, giving 100 as an argument to the function (which didn't work)) ([possible solution](https://www.ibm.com/docs/en/xl-fortran-aix/15.1.2?topic=openmp-omp-set-num-threadsnumber-threads-expr#ompsetnumthreads))
        -   case 2: 1000 threads using compiler directives ([possible solution](https://www.comrevo.com/2016/01/how-to-create-threads-using-openmp-api.html)):
            ```c
            #pragma omp parallel num_threads(7)
            {
                printf("Hello World");
            }
            ```
        -   case 3: 1000 threads using environment variables (done?) ([this?](https://github.com/LLNL/HPC-Tutorials/blob/main/openmp/code_examples/C/omp_hello.c))
    -   open mp can do both low(mutex) and high level synchronization
    -   vector multiplication of 2 arrays and measure the time (for 100 and 10k threads)

# 20220816

-   parallel for loops
-   normal for loops are very sequential
-   omp can be used to makes loops parallel (different iteration on different threads)
-   use:
    ```c
    	#pragma omp for
    	for (...) {
    		...
    	}
    ```
-   We can also task(functional) parallelism in omp using 'Sections'
    ```c
    #pragma omp parallel
    {
    	#pragma omp sections
    	#pragma omp section
    	{
    		... // task 1
    	}
    	#pragma omp section
    	{
    		... // task 2
    	}
    }
    ```
-   Back to loop level parallelism (that was just a tangent i guess)
-   Types of scheduling:
    -   static: predetermined (always the same)
    -   dynamic: have to specify the chunk size
    -   auto
    -   guided
    -   runtime
-   eg:
    ```c
        #pragma omp parallel for schedule(static)
        for (i = 0; i < 10000000; i++) {
            ... // body
        }
        // implied barrier
    ```
-   Implied Barrier:
    -   After the for loop ends, there is an implied 'barrier'
    -   After execution, the threads need to converge, if one thread is slower than the other, then the others reach the barrier and then stop.
    -   The barrier helps in synchronising threads.
-   [link](https://hpc-tutorials.llnl.gov/openmp/work_sharing_constructs/) for further reading
-   assignment:
    -   use omp critical (preprocessor directive) for adding the partial sum to the total one
    -   global sum of even and odd is apparently "sum all even and sum all odd"
    -   wtf was wrong with vector addition :(

# 20220823 | Synchronization Constructs in omp
- Using semaphores and shit
- Any number of threads can read a shared variable, but writing to it causes problems (race conditions)

- Low level synchronization:
    - locks
    - one thread will lock a variable, and no other thread can use that variable unless it has been unlocked.
    - we have to implement some sort of 'fairness' to allow each thread to access the shared variable (for equal amounts of time?)
    - locks can be used using omp
- omp directives for synchronization:
    - `#pragma omp single`:
        - the block will only be executed by a single thread in a team
        - good for I/O stuff
        - there is an implied barrier after a `single` block
    - `#pragma omp master`:
        - the block will only be executed by the master thread(`omp_get_thread_num` == 0)
        - no implicit barrier, the other threads will not wait for the master to finish
    - `#pragma omp critical`:
        - only one thread will execute the block
        - if any other thread reaches it, it will wait for the other thread to finish the criticla section
        - we can ensure that multiple threads do no write to a single value
    - `#pragma omp atomic`:
        - similar to `critical`
        - only used for updating a memory location
        - ensures that a memory location is updated atomically (can't be interrupted?)
    - `#pragma omp ordered`:
        - the block will be executed in order (according to the tid?)
    - `#pramga omp barrier`:
        - to make an explicit barrier
        - all threads will converge when a barrier is reached and the program will continue
- Reductions:
    - `#pragma omp parallel for reduction(+: sum)`
    - eg for loop: `for (j = 0; j < N; j++) sum += a[j] + b[j];`
    - to parallelize the above loop, the sum needs to be updated atomically but that will be too expensive. So, a reduction will keep a private copy and then add (or apply whatever the operation is) every value at the end
    - operators: any associative: +, -, ||, |, ...
- look at the llnlmnop tutorials for synchronization constructs



*samridh > shmiddh


```c

#include <omp.h> 
#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

int main() {
    int i; int max; int a[SIZE];
    for (i = 0; i < SIZE; i++) {
        a[i] = rand();
        printf(
            “a[%d] = %d \t thread no %d\n",
            i,
            a[i],
            omp_get_num_threads()
        );
    } 
    max = a[0]; 
    #pragma omp parallel for  
    for (i = 1; i < SIZE; i++) {
        if (a[i] > max) {
            #pragma omp critical 
            { 
                // compare a[i] and max again because max  could have been changed by another thread after
                // the comparison outside the critical section 
                if (a[i] > max) max = a[i];
             }
        }
    }
    printf("max = %d\t thread no = %d\n", max, omp_get_num_threads()); 
} 
```

# 20220906 | Reductions
- [here](http://jakascorner.com/blog/2016/06/omp-for-reduction.html) is the site sir referred to
- [possible question?](http://jakascorner.com/blog/2016/05/omp-monte-carlo-pi.html)
-  assignments:
    - in q1, compare all 3
    - [code](https://github.com/jakaspeh/concurrency/blob/master/ompMonteCarloPi.cpp) for q3

# 20220913 | Profiling
- Basically, performance analysis
- Used to optimise code
- we will use _ompP_ to profile our omp code
- to use ompp:
    - extract the tarball
    - use `make install` to install
    - ???
    - profit
- do the matrix multiplication assignment (idk which one) and use the profiler on that, get the callgraph and some other shit?
- 
































