#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int send_data, recv_data;
    MPI_Status status;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) {
            printf("This program requires exactly 2 processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    // Assign values to send
    send_data = rank;

    // Create a deadlock situation
    if (rank == 0) {
        // Process 0 is trying to send to Process 1 and receive from Process 1
        MPI_Send(&send_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        printf("Process 0 received data %d from Process 1\n", recv_data);
    } else if (rank == 1) {
        // Process 1 is trying to send to Process 0 and receive from Process 0
        MPI_Send(&send_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Process 1 received data %d from Process 0\n", recv_data);
    }

    // Finalize the MPI environment
    MPI_Finalize();
    return 0;
}
