#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ARRAY_SIZE 10000000  // Adjust the array size as needed

void initialize_array(int *array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 10;
    }
}

int main() {
    int *array = (int*)malloc(ARRAY_SIZE * sizeof(int));
    initialize_array(array, ARRAY_SIZE);

    // Sequential sum
    int total_sum_seq = 0;
    double start_time = omp_get_wtime();
    for (int i = 0; i < ARRAY_SIZE; i++) {
        total_sum_seq += array[i];
    }
    double end_time = omp_get_wtime();
    printf("Sequential sum: %d, Time: %f seconds\n", total_sum_seq, end_time - start_time);

    // Parallel sum using OpenMP reduction clause
    int total_sum_parallel = 0;
    start_time = omp_get_wtime();
    #pragma omp parallel for reduction(+:total_sum_parallel)
    for (int i = 0; i < ARRAY_SIZE; i++) {
        total_sum_parallel += array[i];
    }
    end_time = omp_get_wtime();
    printf("Parallel sum (reduction): %d, Time: %f seconds\n", total_sum_parallel, end_time - start_time);

    free(array);
    return 0;
}
