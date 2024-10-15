#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get the rank of the process
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Get the total number of processes

    int N = 4;  // Size of the matrix (N x N) and the vector (N)

    // Matrix and vector data (only initialized in the root process)
    std::vector<int> matrix;
    std::vector<int> vector(N);
    std::vector<int> result(N, 0);  // Result of matrix-vector multiplication

    if (rank == 0) {
        // Initialize matrix (flattened as a 1D array) and vector (example data)
        matrix = {
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 10, 11, 12,
            13, 14, 15, 16
        };
        vector = {1, 2, 3, 4};
    }

    // Scatter rows of the matrix to each process
    int rows_per_process = N / size;  // Assuming N is divisible by the number of processes
    std::vector<int> local_matrix(rows_per_process * N);  // Buffer for local rows

    MPI_Scatter(matrix.data(), rows_per_process * N, MPI_INT, local_matrix.data(),
                rows_per_process * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast the vector to all processes
    MPI_Bcast(vector.data(), N, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process computes its local result
    std::vector<int> local_result(rows_per_process, 0);
    for (int i = 0; i < rows_per_process; ++i) {
        for (int j = 0; j < N; ++j) {
            local_result[i] += local_matrix[i * N + j] * vector[j];
        }
    }

    // Gather the local results into the final result at root process
    MPI_Gather(local_result.data(), rows_per_process, MPI_INT, result.data(),
               rows_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    // Root process prints the result
    if (rank == 0) {
        std::cout << "Result of Matrix-Vector Multiplication:\n";
        for (int i = 0; i < N; ++i) {
            std::cout << result[i] << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
