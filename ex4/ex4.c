/*
Arsen Davtyan, 714
Task 4: Matrix multiplication
*/

#include "omp.h"
#include "stdio.h"
#include "stdlib.h"
#include "assert.h"

typedef struct {
    double** M;
    int numRow;
    int numCol;
} Matrix;

// Matrix initializing
// depending on isRandom the matrix consists of either random numbers or zeros
double** CreateMatrix(int numRow, int numCol, int isRandom) {
    double** M = (double**)malloc(numRow * sizeof(double*));
    for (int i = 0; i < numRow; i++) {
        M[i] = (double*)malloc(numCol * sizeof(double));
        for (int j = 0; j < numCol; j++) {
            M[i][j] = isRandom ? (double)rand() / RAND_MAX - 0.5 : 0;
        }
    }
    return M;
}

// Parallel matrix multiplication
Matrix MultiplyMatrices(Matrix* leftM, Matrix* rightM) {
    assert(leftM->numCol == rightM->numRow);
    Matrix resM = { .numRow = leftM->numRow, .numCol = rightM->numCol};
    resM.M = CreateMatrix(resM.numRow, resM.numCol, 0);

    #pragma omp parallel
    {
        // each row elemnt is multiplied by its corresponding column elemnt and gets added to the sum
        #pragma omp for schedule(dynamic)
        for (int elemIdx = 0; elemIdx < resM.numRow * resM.numCol; elemIdx++) {
            int elemRow = elemIdx / resM.numCol;
            int elemCol = elemIdx % resM.numCol;

            for (int j = 0; j < leftM->numCol; j++) {
                resM.M[elemRow][elemCol] += leftM->M[elemRow][j] * rightM->M[j][elemCol];
            }
        }
    }

    return resM;
}

int main() {
    Matrix A = { .numRow = 10, .numCol = 7};
    A.M = CreateMatrix(A.numRow, A.numCol, 1);
    Matrix B = { .numRow = 7, .numCol = 8};
    B.M = CreateMatrix(B.numRow, B.numCol, 1);

    Matrix C = MultiplyMatrices(&A, &B);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 8; j ++) {
            printf("%f ", C.M[i][j]);
        }
        printf("\n");
    }

    return 0;
}
