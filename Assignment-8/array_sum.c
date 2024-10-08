#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size, n, i;
    int *A = NULL;
    int local_sum = 0, global_sum = 0;
    
    MPI_Init(&argc, &argv);              // Initialize the MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the number of processes
    
    if (size != 2) {
        if (rank == 0) {
            printf("This program requires exactly 2 processes.\n");
        }
        MPI_Finalize();
        return 1;
    }
    
    if (rank == 0) {
        // Process P0 reads the input size and array elements
        printf("Enter the size of array (even number): ");
        scanf("%d", &n);
        
        if (n % 2 != 0) {
            printf("Array size must be an even number.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        
        A = (int *)malloc(n * sizeof(int));
        printf("Enter %d elements of the array:\n", n);
        for (i = 0; i < n; i++) {
            scanf("%d", &A[i]);
        }
    }
    
    // Broadcast the size of the array to all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Allocate memory for the local half of the array
    int half_n = n / 2;
    int *local_A = (int *)malloc(half_n * sizeof(int));
    
    // Scatter the array into two equal parts between P0 and P1
    MPI_Scatter(A, half_n, MPI_INT, local_A, half_n, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Each process computes the sum of its part
    for (i = 0; i < half_n; i++) {
        local_sum += local_A[i];
    }
    
    // Use MPI_Reduce to sum the results from both processes
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    // Process P0 prints the final result
    if (rank == 0) {
        printf("The total sum of array elements is: %d\n", global_sum);
        free(A);
    }
    
    // Free dynamically allocated memory
    free(local_A);
    
    MPI_Finalize(); // Finalize the MPI environment
    return 0;
}
