/*
Arsen Davtyan, 714
Task 2: Sum natural number from 1 to N
Additional: N is set by command-line arguments
*/

#include "omp.h"
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char* argv[])
{
    // get N and number of threads from command line
    int N = 0;
    int threadCount = 0;
    if(argc == 3 && atoi(argv[1]) >= 1 && atoi(argv[2]) >= 1)
    {
        N = atoi(argv[1]);
        threadCount = atoi(argv[2]);
    }
    else
    {
        printf("Invalid input\n./out N THREAD_NUM\n");
        return -1;
    }

    int i = 0;
    int sum = 0;

    // initialize partial sums of each thread with 0
    int* partialSums = (int*)malloc(threadCount * sizeof(int));
    for (i = 0; i < threadCount; i++)
    {
        partialSums[i] = 0;
    }

    // start parallel execution
    #pragma omp parallel num_threads(threadCount)
    {

        // calculate partial sums of each thread in a parallel loop
        #pragma omp for private(i)
            for (i = 1; i <= N; i++)
            {
                int tid = omp_get_thread_num();
                partialSums[tid] += i;
            }

        // one thread adds partial sums together
        #pragma omp single
        {
            for (i = 0; i < threadCount; i++)
                sum += partialSums[i];
        }
    }

    free(partialSums);

    printf("N = %d\nResult: %d\n", N, sum);

    return 0;
}
