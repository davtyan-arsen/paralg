/******************************************************************************
* ЗАДАНИЕ: bugged2.c
* ОПИСАНИЕ:
*   Еще одна программа на OpenMP с багом.
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int nthreads, i, tid;
    float total;

    // Should specify shared and private variables
    // If we want to know totals of each thread the variable total should be private
    // otherwise put it in shared and uncomment the reduction condition and the barrier before omp for

    //#pragma omp parallel
    #pragma omp parallel shared(nthreads) private(i, tid, total)
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        printf("Thread %d is starting...\n", tid);

        #pragma omp barrier

        total = 0.0;
        //#pragma omp barrier
        //to avoid undefined behaviour

        #pragma omp for schedule(dynamic, 10)
        //reduction(+:total)
        for (i = 0; i < 1000000; i++)
            total = total + i*1.0;

        printf ("Thread %d is done! Total= %e\n", tid, total);
    }
}
