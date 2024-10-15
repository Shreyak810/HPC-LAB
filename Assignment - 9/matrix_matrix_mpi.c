
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size, m, n, p, local_m;
    double start_time, end_time;
    int *matrix_a = NULL;
    int *matrix_b = NULL;
    int *local_a = NULL;
    int *local_b = NULL;
    int *local_c = NULL;
    int *result = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter dimensions for matrix A (m x n) and matrix B (n x p):\n");
        scanf("%d %d %d", &m, &n, &p);
        
        // Broadcast dimensions to all processes
        MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&p, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        // Other ranks receive the dimensions
        MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&p, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Check if m is divisible by size
    if (m % size != 0) {
        if (rank == 0) {
            printf("Matrix rows (m) must be divisible by the number of processes.\n");
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    local_m = m / size;

    // Allocate memory for the matrices
    matrix_a = (int *)malloc(m * n * sizeof(int));
    matrix_b = (int *)malloc(n * p * sizeof(int));
    local_a = (int *)malloc(local_m * n * sizeof(int));
    local_b = (int *)malloc(n * p * sizeof(int));
    local_c = (int *)malloc(local_m * p * sizeof(int));

    // Input matrices from all processes
    if (rank == 0) {
        printf("Enter elements for matrix A:\n");
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                scanf("%d", &matrix_a[i * n + j]);
            }
        }

        printf("Enter elements for matrix B:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < p; j++) {
                scanf("%d", &matrix_b[i * p + j]);
            }
        }
    }

    // Broadcast matrix B to all processes
    MPI_Bcast(matrix_b, n * p, MPI_INT, 0, MPI_COMM_WORLD);

    // Scatter matrix A to all processes
    MPI_Scatter(matrix_a, local_m * n, MPI_INT, local_a, local_m * n, MPI_INT, 0, MPI_COMM_WORLD);

    start_time = MPI_Wtime();

    // Local computation for matrix multiplication
    for (int i = 0; i < local_m; i++) {
        for (int j = 0; j < p; j++) {
            local_c[i * p + j] = 0;
            for (int k = 0; k < n; k++) {
                local_c[i * p + j] += local_a[i * n + k] * matrix_b[k * p + j];
            }
        }
    }

    // Gather results from all processes
    if (rank == 0) {
        result = (int *)malloc(m * p * sizeof(int));
    }
    MPI_Gather(local_c, local_m * p, MPI_INT, result, local_m * p, MPI_INT, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();

    // Print the result on rank 0
    if (rank == 0) {
        printf("Matrix-Matrix multiplication result:\n");
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < p; j++) {
                printf("%d ", result[i * p + j]);
            }
            printf("\n");
        }
        printf("Execution Time: %f seconds\n", end_time - start_time);
        free(matrix_a);
        free(matrix_b);
        free(result);
    }

    // Free allocated memory
    free(local_a);
    free(local_b);
    free(local_c);

    MPI_Finalize();
    return 0;
}
