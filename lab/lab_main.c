#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "time.h"

#define ISIZE 400
#define JSIZE 1000

int main(int argc, char **argv)
{
    clock_t begin = clock();

    double a[ISIZE][JSIZE];
    int i, j;
    FILE *ff;

    for (i=0; i<ISIZE; i++){
        for (j=0; j<JSIZE; j++){
            a[i][j] = 10*i +j;
        }
    }

    for (i=0; i<ISIZE; i++){
        for (j = 0; j < JSIZE; j++){
            a[i][j] = sin(0.00001*a[i][j]);
        }
    }

    clock_t end = clock();
    double walltime = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("walltime - %fs \n", walltime);

    ff = fopen("result_main.txt","w");

    for(i=0; i < ISIZE; i++){
        for (j=0; j < JSIZE; j++){
            fprintf(ff,"%f ",a[i][j]);
        }
        fprintf(ff,"\n");
    }

    fclose(ff);

}
