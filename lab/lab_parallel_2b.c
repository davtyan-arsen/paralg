#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#define ISIZE 400
#define JSIZE 1000

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    double walltime = MPI_Wtime();

    int rank, NP;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &NP);

    // Initializing the computation borders for each thread
    int segmentSize = ISIZE / NP;
    int leftBound = rank * segmentSize;
    int rightBound = (rank + 1) * segmentSize;
    if (rank == NP - 1)
        rightBound = ISIZE;

    double* data[ISIZE];
    double* buffer[3];
    for (int i = 0; i < 3; i++) {
        buffer[i] = (double*)malloc(JSIZE * sizeof(double));
    }

    // Allocating memory for each thread
    if (rank == 0) {
        for (int i = 0; i < ISIZE; i++) {
            data[i] = (double*)malloc(JSIZE * sizeof(double));
        }
    }
    else {
        for (int i = 0; i < rightBound - leftBound; i++)
            data[i] = (double*)malloc(JSIZE * sizeof(double));
    }

    // computation: step 1
    int k = 0;
    for (int i = leftBound; i < rightBound; i++) {
        for (int j = 0; j < JSIZE; j++) {
            k = i % segmentSize;
            data[k][j] = 10*i + j;
        }
    }

    // Each thread gets the values it needs from the next thread (n gets from n+1)
    if (rank != 0) {
        MPI_Ssend(&data[0][0], JSIZE, MPI_DOUBLE, rank - 1, rank - 1, MPI_COMM_WORLD);
        MPI_Ssend(&data[1][0], JSIZE, MPI_DOUBLE, rank - 1, rank - 1, MPI_COMM_WORLD);
        MPI_Ssend(&data[2][0], JSIZE, MPI_DOUBLE, rank - 1, rank - 1, MPI_COMM_WORLD);
    }
    if (rank != NP - 1) {
        MPI_Recv(&buffer[0][0], JSIZE, MPI_DOUBLE, rank + 1, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&buffer[1][0], JSIZE, MPI_DOUBLE, rank + 1, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&buffer[2][0], JSIZE, MPI_DOUBLE, rank + 1, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }


    // computation: step 2
    for (int i = leftBound; i < rightBound; i++) {
        for (int j = 2; j < JSIZE; j++) {
            k = i % segmentSize;
            if (i + 3 >= rightBound) {
                data[k][j] = sin(0.00001 * buffer[k][j-2]);
            } else {
                data[k][j] = sin(0.00001 * data[k+3][j-2]);
            }
        }
    }

    int receiveSize = segmentSize;
    if (rank == 0) {
        for (int i = 1; i < NP; i++) {
            if (i == NP-1)
                receiveSize = ISIZE - (NP - 1) * segmentSize;

            int leftBound = i * segmentSize;

            // The master thread gets the computation results from each thread
            for (k = 0; k < receiveSize; k++) {
                MPI_Recv(&data[leftBound+k][0], JSIZE, MPI_DOUBLE, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    }
    else {
        // Slave threads send their computation results to the master thread
        if (rank == NP-1) {
            receiveSize = ISIZE - (NP - 1) * segmentSize;
        }

        for (k = 0; k < receiveSize; k++) {
            MPI_Ssend(&data[k][0], JSIZE, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        // printing walltime and writing the results into a file
        walltime = MPI_Wtime() - walltime;
        printf("walltime - %fs \n", walltime);

        FILE* f = fopen("results_parallel_2b.txt", "w");
        for(int i = 0; i < ISIZE; i++){
            for (int j = 0; j < JSIZE; j++){
                fprintf(f,"%lf ", data[i][j]);
            }
            fprintf(f,"\n");
        }
        fclose(f);

        // Freeing the allocated memory
        for (int i = 0; i < ISIZE; i++) {
            free(data[i]);
        }
    } else {
        for (int i = 0; i < rightBound - leftBound; i++)
            free(data[i]);
    }

    MPI_Finalize();

    return 0;
}
