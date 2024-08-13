#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Function to generate a random vector
void generate_vector(int N, int vector[N]) {
    for (int i = 0; i < N; i++) {
        vector[i] = rand() % 100;
    }
}

// Function to add a scalar to each element of the vector using STATIC schedule
void vector_scalar_add_static(int N, int vector[N], int scalar, int num_threads, int chunk_size) {
    #pragma omp parallel for num_threads(num_threads) schedule(static, chunk_size)
    for (int i = 0; i < N; i++) {
        vector[i] += scalar;
    }
}

// Function to add a scalar to each element of the vector using DYNAMIC schedule
void vector_scalar_add_dynamic(int N, int vector[N], int scalar, int num_threads, int chunk_size) {
    #pragma omp parallel for num_threads(num_threads) schedule(dynamic, chunk_size)
    for (int i = 0; i < N; i++) {
        vector[i] += scalar;
    }
}

// Function to measure the time taken for vector-scalar addition
double measure_time_static(int N, int scalar, int num_threads, int chunk_size) {
    int vector[N];
    generate_vector(N, vector);

    clock_t start_time = clock();
    vector_scalar_add_static(N, vector, scalar, num_threads, chunk_size);
    clock_t end_time = clock();

    return (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

double measure_time_dynamic(int N, int scalar, int num_threads, int chunk_size) {
    int vector[N];
    generate_vector(N, vector);

    clock_t start_time = clock();
    vector_scalar_add_dynamic(N, vector, scalar, num_threads, chunk_size);
    clock_t end_time = clock();

    return (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

int main() {
    int N = 200;  // Size of the vector
    int scalar = 10;  // Scalar to be added
    int threads[] = {1, 2, 4, 8};  // Number of threads to use
    int chunk_sizes[] = {1, 5, 10, 20};  // Different chunk sizes for scheduling

    printf("STATIC Schedule:\n");
    printf("Threads\tChunk Size\tTime(s)\n");
    for (int i = 0; i < 4; i++) {
        int num_threads = threads[i];
        for (int j = 0; j < 4; j++) {
            int chunk_size = chunk_sizes[j];
            double time_taken = measure_time_static(N, scalar, num_threads, chunk_size);
            printf("%d\t%d\t\t%f\n", num_threads, chunk_size, time_taken);
        }
    }

    printf("\nDYNAMIC Schedule:\n");
    printf("Threads\tChunk Size\tTime(s)\n");
    for (int i = 0; i < 4; i++) {
        int num_threads = threads[i];
        for (int j = 0; j < 4; j++) {
            int chunk_size = chunk_sizes[j];
            double time_taken = measure_time_dynamic(N, scalar, num_threads, chunk_size);
            printf("%d\t%d\t\t%f\n", num_threads, chunk_size, time_taken);
        }
    }

    return 0;
}
