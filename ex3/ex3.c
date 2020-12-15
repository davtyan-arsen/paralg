/*
Arsen Davtyan, 714
Task 3: Data Dependency
*/

#define ARRAY_LEN 100000

#include "omp.h"
#include "stdio.h"
#include "stdlib.h"

int main() {
    unsigned long a[ARRAY_LEN];
    unsigned long long b[ARRAY_LEN];

    int i = 0;
    for (i = 0; i < ARRAY_LEN; i++)
        a[i] = i + 1;

    #pragma omp parallel
    {
        #pragma omp for private(i)
        for (i = 1; i < ARRAY_LEN - 1; i++) {
            b[i] = ( a[i-1] * a[i] * a[i+1]) / 3.0;
        }

        #pragma omp single
        {
            b[0] = 0;
            b[ARRAY_LEN - 1] = ( a[ARRAY_LEN - 2] * a[ARRAY_LEN - 1] * (ARRAY_LEN + 1) ) / 3;
        }
    }

    // printing the first and last 5 elements
    for (i = 0; i < ARRAY_LEN; i++) {
        if (i < 5 || i > ARRAY_LEN - 5) {
            printf("a[%d] = %ld    b[%d] = %lld\n", i, a[i], i, b[i]);

        }
    }

    return 0;
}
