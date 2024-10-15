#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void fill_matrix(double *matrix, int rows, int cols) {
    printf("Enter the elements of the matrix (%d x %d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            scanf("%lf", &matrix[i * cols + j]);
        }
    }
}

void fill_vector(double *vector, int size) {
    printf("Enter the elements of the vector (%d):\n", size);
    for (int i = 0; i < size; i++) {
        printf("Element [%d]: ", i);
        scanf("%lf", &vector[i]);
    }
}

void print_vector(double *vector, int size) {
    printf("Result vector:\n");
    for (int i = 0; i < size; i++) {
        printf("%f ", vector[i]);
    }
    printf("\n");
}

int main(int argc, char **argv) {
    int rank, size;
    int n, m; // Number of rows and columns
    double *matrix = NULL;
    double *vector = NULL;
    double *local_matrix = NULL;
    double *local_result = NULL;
    double *result = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Get matrix dimensions from user
        printf("Enter the number of rows in the matrix: ");
        scanf("%d", &n);
        printf("Enter the number of columns in the matrix: ");
        scanf("%d", &m);

        // Allocate and fill matrix and vector
        matrix = (double *)malloc(n * m * sizeof(double));
        vector = (double *)malloc(m * sizeof(double));
        result = (double *)malloc(n * sizeof(double));
        
        fill_matrix(matrix, n, m);
        fill_vector(vector, m);
    }

    // Broadcast the dimensions of the matrix and the vector
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Allocate space for the local matrix and result
    int rows_per_process = n / size;
    local_matrix = (double *)malloc(rows_per_process * m * sizeof(double));
    local_result = (double *)malloc(rows_per_process * sizeof(double));

    // Scatter the rows of the matrix to all processes
    MPI_Scatter(matrix, rows_per_process * m, MPI_DOUBLE, local_matrix, rows_per_process * m, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Broadcast the vector to all processes
    MPI_Bcast(vector, m, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Perform local matrix-vector multiplication
    for (int i = 0; i < rows_per_process; i++) {
        local_result[i] = 0;
        for (int j = 0; j < m; j++) {
            local_result[i] += local_matrix[i * m + j] * vector[j];
        }
    }

    // Gather the results from all processes
    MPI_Gather(local_result, rows_per_process, MPI_DOUBLE, result, rows_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Print the result from the root process
    if (rank == 0) {
        print_vector(result, n);
        // Free allocated memory
        free(matrix);
        free(vector);
        free(result);
    }

    // Free local memory
    free(local_matrix);
    free(local_result);

    MPI_Finalize();
    return 0;
}
