/*
Arsen Davtyan, 714
Task 1: "Hello world!"
Additional: Threads display their ID-s in reverse order
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

    // initialize the first thread id to be displayed
    int currentThread = threadCount - 1;

    #pragma omp parallel num_threads(threadCount) shared(currentThread)
    {
        int tid = omp_get_thread_num();

        // wait for the previous thread to display its id
        while (tid < currentThread) {};

        #pragma omp critical
        {
            printf("Hello world! This is thread %d \n", tid);
            // update the current thread
            currentThread -= 1;
        }
    }

    return 0;
}
