#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fill_array(double *array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 100;  // Fill array with random values
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Define message sizes to test
    int message_sizes[] = {1000, 10000, 100000, 1000000}; // Example sizes
    int num_sizes = sizeof(message_sizes) / sizeof(message_sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        int size = message_sizes[s];
        double *data = (double*)malloc(size * sizeof(double));
        double local_sum = 0.0, global_sum = 0.0;

        // Fill the array with random numbers only in rank 0
        if (world_rank == 0) {
            fill_array(data, size);
        }

        // Broadcast data to all processes
        MPI_Bcast(data, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Start timing
        double start_time = MPI_Wtime();

        // Each process computes its local sum
        for (int i = 0; i < size; i++) {
            local_sum += data[i];
        }

        // Perform reduction operation to calculate global sum
        MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        // Stop timing
        double end_time = MPI_Wtime();

        // Rank 0 prints the result and time taken
        if (world_rank == 0) {
            printf("Size: %d, Global Sum: %f, Time taken: %f seconds\n", size, global_sum, end_time - start_time);
        }

        free(data);
    }

    MPI_Finalize();
    return 0;
}

