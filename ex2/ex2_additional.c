/*
Arsen Davtyan, 714
Task 2: Sum natural number from 1 to N
Additional: reduction and schedule conditions are used + N is set by command-line arguments
*/

#include "omp.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

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

    int sum = 0;
    int i = 0;

    // initialize chunk size for dynamic parallel for loop (min = 1)
    int chunkSize = N / threadCount / 4;
    if (chunkSize == 0)
        chunkSize = 1;

    // start parallel execution
    #pragma omp parallel num_threads(threadCount) shared(sum)
    {

        #pragma omp for private(i) reduction(+:sum) schedule(dynamic, chunkSize)
            for (i = 1; i <= N; i++)
            {
                sum += i;
            }
    }

    printf("N = %d\nResult: %d\n", N, sum);

    return 0;
}
