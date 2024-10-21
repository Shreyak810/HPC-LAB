#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int numtasks, rank;
    int root = 0;
    int messageSize = 1000000;  // Constant message size (fixed size broadcast)
    int* data = (int*)malloc(messageSize * sizeof(int));

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Initialize data only on the root process
    if (rank == root) {
        for (int i = 0; i < messageSize; i++) {
            data[i] = i;
        }
    }

    // Measure the broadcast time
    double start_time = MPI_Wtime();

    // Broadcast data from root to all processes
    MPI_Bcast(data, messageSize, MPI_INT, root, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    // Print the time taken on each process
    if (rank == root) {
        printf("Time taken for broadcast with %d processes: %f seconds\n", numtasks, end_time - start_time);
    }

    // Finalize the MPI environment
    MPI_Finalize();

    free(data);
    return 0;
}

