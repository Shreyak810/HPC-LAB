#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void matrix_vector_multiply(int** A, int* x, int* y, int N) {
    int i, j;

    // Parallelizing the loop with OpenMP
    #pragma omp parallel for private(i, j) shared(A, x, y)
    for (i = 0; i < N; i++) {
        y[i] = 0;  // Initialize result element
        for (j = 0; j < N; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
}

int main() {
    int N;
    printf("Enter the size of the matrix (N x N) and vector (N): ");
    scanf("%d", &N);

    // Dynamically allocate memory for the matrix and vectors
    int** A = (int**)malloc(N * sizeof(int*));
    int* x = (int*)malloc(N * sizeof(int));
    int* y = (int*)malloc(N * sizeof(int));

    for (int i = 0; i < N; i++) {
        A[i] = (int*)malloc(N * sizeof(int));
    }

    // Input matrix A
    printf("Enter elements of matrix A:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    // Input vector x
    printf("Enter elements of vector x:\n");
    for (int i = 0; i < N; i++) {
        scanf("%d", &x[i]);
    }

    // Perform matrix-vector multiplication
    matrix_vector_multiply(A, x, y, N);

    // Print result
    printf("Matrix A * Vector x = Vector y:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", y[i]);
    }
    printf("\n");

    // Free the allocated memory
    for (int i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    free(x);
    free(y);

    return 0;
}
