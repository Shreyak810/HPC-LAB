#include <stdio.h>
#include <omp.h>

int main() {
    int n = 20;
    int total_sum = 0;
    int partial_sum = 0; // Declare partial_sum outside the parallel region

    #pragma omp parallel
    {
        // Initialize partial_sum to 0 within the parallel region
        partial_sum = 0;

        #pragma omp for lastprivate(partial_sum)
        for (int i = 1; i <= n; i++) {
            partial_sum += i;
        }

        #pragma omp critical
        total_sum += partial_sum;
    }

    printf("Total sum: %d\n", total_sum);

    return 0;
}
