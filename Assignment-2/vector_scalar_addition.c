#include <stdio.h>
#include <omp.h>

int main() {
    int N = 10000000; // Size of the vector
    int scalar = 5;   // Scalar value to add
    int vec[N];       // Vector of size N

    // Initialize the vector with some values (e.g., all 1s)
    for (int i = 0; i < N; i++) {
        vec[i] = 1;
    }


    // Parallel vector-scalar addition
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        vec[i] += scalar;
    }

     // Print the resulting vector
    printf("Resulting vector: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", vec[i]);
    }
    printf("\n");
    return 0;
}
