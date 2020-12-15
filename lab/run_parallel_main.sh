mpicc lab_parallel_main.c -lm -o out
mpirun -np 6 ./out
