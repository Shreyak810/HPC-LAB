#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Function to generate a random matrix
void generate_matrix(int N, int **matrix) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
}

// Function to add two matrices
void add_matrices(int N, int **A, int **B, int **C, int num_threads) {
    #pragma omp parallel for num_threads(num_threads) collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

// Function to measure the time taken for matrix addition
double measure_time(int N, int num_threads) {
    // Allocate memory for matrices dynamically
    int *A = (int *)malloc(N * sizeof(int *));
    int *B = (int *)malloc(N * sizeof(int *));
    int *C = (int *)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        A[i] = (int *)malloc(N * sizeof(int));
        B[i] = (int *)malloc(N * sizeof(int));
        C[i] = (int *)malloc(N * sizeof(int));
    }

    generate_matrix(N, A);
    generate_matrix(N, B);

    clock_t start_time = clock();
    add_matrices(N, A, B, C, num_threads);
    clock_t end_time = clock();

    // Free allocated memory
    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

int main() {
    int sizes[] = {250, 500, 750, 1000, 2000};
    int threads[] = {1, 2, 4, 8};

    printf("Matrix Size\tThreads\tTime(s)\n");

    for (int i = 0; i < 5; i++) {
        int N = sizes[i];
        for (int j = 0; j < 4; j++) {
            int num_threads = threads[j];
            double time_taken = measure_time(N, num_threads);
            printf("%d\t\t%d\t%f\n", N, num_threads, time_taken);
        }
    }

    return 0;
}
