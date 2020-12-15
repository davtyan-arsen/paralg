gcc -fopenmp ex5.c -lm -o out
OMP_NUM_THREADS=5 ./out 2 10
