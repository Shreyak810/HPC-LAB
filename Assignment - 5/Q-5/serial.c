#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void matrixMultiplySerial(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int n = 3; // Size of the matrix

    // Dynamically allocate memory for matrices
    int** A = (int**)malloc(n * sizeof(int*));
    int** B = (int**)malloc(n * sizeof(int*));
    int** C = (int**)malloc(n * sizeof(int*));

    for (int i = 0; i < n; ++i) {
        A[i] = (int*)malloc(n * sizeof(int));
        B[i] = (int*)malloc(n * sizeof(int));
        C[i] = (int*)malloc(n * sizeof(int));
    }

    // Initialize matrix A
    int initA[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    // Initialize matrix B
    int initB[3][3] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };

    // Copy values from initA and initB to A and B
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i][j] = initA[i][j];
            B[i][j] = initB[i][j];
        }
    }

    // Measure the execution time
    clock_t start = clock();
    matrixMultiplySerial(A, B, C, n);
    clock_t end = clock();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Serial Execution Time: %f seconds\n", time_spent);

    // Output matrix C
    printf("Matrix C:\n");
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    // Free dynamically allocated memory
    for (int i = 0; i < n; ++i) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
