#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void function1() {
    // int n = 100000;
    int n = 16;
    int *nums = (int *)calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        nums[i] = rand() % 100;
    }

    int i = 0;
    int local_min_1, local_min_2, local_min_3, local_min_4;
    int min = 100;

    #pragma omp parallel shared(nums, n, min) private(i, local_min_1, local_min_2, local_min_3, local_min_4) num_threads(4)
    {
        #pragma omp sections
        {
            min = 100;
            #pragma omp section
            {
                local_min_1 = 100;
                for (i = 0; i < n/4; i++) {
                    if (nums[i] < local_min_1) local_min_1 = nums[i];
                }
                #pragma omp critical
                {
                    if (local_min_1 < min) min = local_min_1;
                }
            }
            #pragma omp section
            {
                local_min_2 = 100;
                for (i = n/4; i < 2 * n/4; i++) {
                    if (nums[i] < local_min_2) local_min_2 = nums[i];
                }
                #pragma omp critical
                {
                    if (local_min_2 < min) min = local_min_2;
                }
            }
            #pragma omp section
            {
                local_min_3 = 100;
                for (i = 2 * n/4; i < 3 * n/4; i++) {
                    if (nums[i] < local_min_3) local_min_3 = nums[i];
                }
                #pragma omp critical
                {
                    if (local_min_3 < min) min = local_min_3;
                }
            }
            #pragma omp section
            {
                local_min_4 = 100;
                for (i = 3 * n/4; i < n; i++) {
                    if (nums[i] < local_min_4) local_min_4 = nums[i];
                }
                #pragma omp critical
                {
                    if (local_min_4 < min) min = local_min_4;
                }
            }
        }
    }

        for (i = 0; i < n; i++) {
            printf(
                "%s%d%s",
                i == 0 ? "[" : " ",
                nums[i],
                i == n - 1 ? "]\n" : ","
            );
        }
        printf("\n%d is the minimum element", min);
        free(nums);
}

int main() {
    function1();

    printf("\n");
    return 0;
}