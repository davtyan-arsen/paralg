/*
Arsen Davtyan, 714
Task 1: "Hello world!"
*/

#include "omp.h"
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char* argv[])
{
    // get number of threads from command line
    int threadCount = 0;
    if(argc == 2 && atoi(argv[1]) >= 1)
        threadCount = atoi(argv[1]);
    else
    {
        printf("Invalid input\n./out THREAD_NUM\n");
        return -1;
    }

    // start parallel execution
    #pragma omp parallel num_threads(threadCount)
    {
        int tid = omp_get_thread_num();
        printf("Hello world! This is thread %d \n", tid);
    }

    return 0;
}
