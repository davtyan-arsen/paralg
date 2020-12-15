/******************************************************************************
* ЗАДАНИЕ: bugged6.c
* ОПИСАНИЕ:
*   Множественные ошибки компиляции
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define VECLEN 100

float a[VECLEN], b[VECLEN];

// float dotprod() // another option is making sum global
float dotprod(float* sum)
{
    int i, tid;
    //float sum;
    float temp_sum = 0;

    #pragma omp parallel private(tid, i)
    {

        tid = omp_get_thread_num();
        //#pragma omp for reduction(+:sum)
        #pragma omp for reduction(+:temp_sum)
        for (i = 0; i < VECLEN; i++)
        {
            // sum = sum + (a[i] * b[i]);
            temp_sum = temp_sum + (a[i] * b[i]);
            printf("  tid= %d i=%d\n", tid, i);
        }
    }

    *sum = temp_sum;
}


int main (int argc, char *argv[])
{
    int i;
    float sum;

    for (i = 0; i < VECLEN; i++)
        a[i] = b[i] = 1.0 * i;
    sum = 0.0;

    // #pragma omp parallel shared(sum) // put parallel in dotprod()
    dotprod(&sum); // pass &sum to dotprod

    printf("Sum = %f\n",sum);
}
