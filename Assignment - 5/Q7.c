#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ARRAY_SIZE 1000000

void initialize_array(int *array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 10;
    }
}

int main() {
    int *array = (int*)malloc(ARRAY_SIZE * sizeof(int));
    initialize_array(array, ARRAY_SIZE);

    int total_sum_unsync = 0;
    int total_sum_critical = 0;
    int total_sum_atomic = 0;

    double start_time, end_time;

    // Unsynchronized version
    start_time = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < ARRAY_SIZE; i++) {
        total_sum_unsync += array[i];  // Race condition here
    }
    end_time = omp_get_wtime();
    printf("Unsynchronized sum: %d, Time: %f seconds\n", total_sum_unsync, end_time - start_time);

    // Synchronized using 'critical' directive
    start_time = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < ARRAY_SIZE; i++) {
        #pragma omp critical
        {
            total_sum_critical += array[i];
        }
    }
    end_time = omp_get_wtime();
    printf("Synchronized sum (critical): %d, Time: %f seconds\n", total_sum_critical, end_time - start_time);

    // Synchronized using 'atomic' directive
    start_time = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < ARRAY_SIZE; i++) {
        #pragma omp atomic
        total_sum_atomic += array[i];
    }
    end_time = omp_get_wtime();
    printf("Synchronized sum (atomic): %d, Time: %f seconds\n", total_sum_atomic, end_time - start_time);

    free(array);
    return 0;
}
