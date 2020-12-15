gcc -fopenmp ex6.c -lm -o out
OMP_NUM_THREADS=5 ./out 9
