/*
Arsen Davtyan, 714
Task 5: Parallel exponent calculation (Taylor series)
*/

/*
    exp(x) = 1 + x/1! + x^2 /2! + ... = sum(n=0;inf) { x^n /n! } ; R = inf
    !!! compile with -lm (for pow)
*/

#include "omp.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "assert.h"
#include "math.h"
#include "float.h"

unsigned long long factorial (int n) {
    assert(n >= 0);
    unsigned long long res = 1;
    int i = 0;
    for (i = 1; i <= n; ++i) {
        res *= i;
    }
    return res;
}

// exp_n(x, n) calculates the n-th elemnt of the Taylor series in point x
double exp_n(double x, int n) {
    unsigned long long fact = factorial(n);
    if (fact < 0) {
        // factorial value too big (out of range)
        return 0;
    }
    double res = pow(x, n) / fact;
    if (!(res >= -DBL_MAX && res <= DBL_MAX)) {
        // starting from a certain value of n and provided |x| > 1
        // pow(x, n) gets out of bounds and becomes INFINITY
        // also dividing by INFINITY sometimes results into NAN
        return 0;
    }
    return res;
}

int main(int argc, const char* argv[]) {
    double x = 0;
    int N = 0;
    if(argc == 3 && atoi(argv[2]) >= 1)
    {
        x = atof(argv[1]);
        N = atoi(argv[2]);
    }
    else
    {
        printf("Invalid input\n./out x N\n");
        return -1;
    }

    double sum = 0;
    int i = 0;

    #pragma omp parallel shared(sum)
    {
        #pragma omp for private(i) reduction(+:sum) schedule(static)
        for (i = 0; i < N; i++)
        {
            sum += exp_n(x, i);
        }
    }

    printf("x = %f\n", x);
    printf("exp(x) = %f\n", exp(x));
    printf("calculated exp(x) = %f\n", sum);

    return 0;
}
