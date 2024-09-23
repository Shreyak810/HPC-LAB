#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the rank of the current process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the total number of processes
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ensure only five processes are run
    if (size != 5) {
        if (rank == 0) {
            printf("This program must be run with exactly 5 processes.\n");
        }
        MPI_Finalize();
        return 1; // Exit with an error code
    }

    // Display the rank and communicator group for each process
    printf("Process %d of %d is part of communicator group MPI_COMM_WORLD\n", rank, size);

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
