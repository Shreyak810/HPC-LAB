#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int left, right;
    int send_data, recv_data_from_left, recv_data_from_right;
    MPI_Status status;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Set up the neighbors in the ring topology
    left = (rank - 1 + size) % size;  // Left neighbor (previous process in the ring)
    right = (rank + 1) % size;        // Right neighbor (next process in the ring)

    // Assign a value to send (each process sends its rank as data)
    send_data = rank;

    // Each process sends to the right and receives from the left
    // Send to the right neighbor
    MPI_Send(&send_data, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
    // Receive from the left neighbor
    MPI_Recv(&recv_data_from_left, 1, MPI_INT, left, 0, MPI_COMM_WORLD, &status);

    // Each process also receives from the right and sends to the left
    MPI_Send(&send_data, 1, MPI_INT, left, 1, MPI_COMM_WORLD);
    MPI_Recv(&recv_data_from_right, 1, MPI_INT, right, 1, MPI_COMM_WORLD, &status);

    // Print the received data
    printf("Process %d received %d from left neighbor %d and %d from right neighbor %d\n",
           rank, recv_data_from_left, left, recv_data_from_right, right);

    // Finalize the MPI environment
    MPI_Finalize();
    return 0;
}
