#include <stdio.h>
#include <omp.h>

int main() {
    int Aryabhatta = 10;  // Variable declared as 10

    int num_threads = 4;  // Define the number of threads

    // Set the number of threads
    omp_set_num_threads(num_threads);

    // Parallel region
    #pragma omp parallel private(Aryabhatta)
    {
        int thread_id = omp_get_thread_num();  // Get the thread ID
        Aryabhatta = 10;  // Initialize Aryabhatta inside the parallel region

        // Compute multiplication of thread ID and Aryabhatta
        int result = thread_id * Aryabhatta;

        // Print the result
        printf("Thread ID: %d, Aryabhatta: %d, Result: %d\n", thread_id, Aryabhatta, result);
    }

    return 0;
}
