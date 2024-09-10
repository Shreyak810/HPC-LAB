#include <stdio.h>
#include <omp.h>

int main() {
    int sum_of_squares = 0;
    int num_threads = 4;  // Define the number of threads

    // Set the number of threads
    omp_set_num_threads(num_threads);

    // Parallel region for calculating square of thread IDs
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();  // Get the thread ID
        int square = thread_id * thread_id;    // Square the thread ID

        // Print the square of the thread ID
        printf("Thread ID: %d, Square: %d\n", thread_id, square);

        // Use atomic to safely accumulate the sum of squares across threads
        #pragma omp atomic
        sum_of_squares += square;
    }

    // Print the sum of the squares of thread IDs
    printf("Sum of squares of thread IDs: %d\n", sum_of_squares);

    return 0;
}
