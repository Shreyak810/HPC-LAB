#include <stdio.h>
#include <omp.h>
int main() {
    int num_threads;

    // Ask the user for the number of threads
    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);

    // Sequential print: "Hello, World" num_threads times
    printf("Sequential output:\n");
    for (int i = 0; i < num_threads; i++) {
        printf("Hello, World\n");
    }
  
    // Parallel print: "Hello, World" from each thread
    printf("\nParallel output:\n");
    omp_set_num_threads(num_threads);
    #pragma omp parallel
    {
        printf("Hello, World from thread %d\n", omp_get_thread_num());
    }
    return 0;
}
