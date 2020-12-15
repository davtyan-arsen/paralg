 /*
 Arsen Davtyan, 714
 Task 6: Prime number count
 */

 #include "omp.h"
 #include "stdio.h"
 #include "stdlib.h"
 #include "math.h"

// Checl if given number is a prime number
int IsPrime(int n) {
    int i = 0;
    for (i = 2; i <= sqrt(n); i++) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

 int main(int argc, const char* argv[]) {
     int N = 0;
     if(argc == 2 && atoi(argv[1]) >= 1)
     {
         N = atoi(argv[1]);
     }
     else
     {
         printf("Invalid input\n./out N // N > 0\n");
         return -1;
     }

     int countPrime = 0;
     int i = 0;

     #pragma omp parallel shared(countPrime)
     {
         #pragma omp for private(i) reduction(+:countPrime) schedule(static)
         for (i = 2; i <= N; i++)
         {
             if (IsPrime(i)) {
                 printf("%d is prime\n", i);
                 countPrime += 1;
             }
         }
     }

     printf("there are %d prime numbers <= %d\n", countPrime, N);

     return 0;
 }
