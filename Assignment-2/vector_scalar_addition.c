#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int N;       // Size of the vector
    int scalar = 5;   // Scalar value to add

    // Take user input for the size of the vector
    printf("Enter the size of the vector: ");
    scanf("%d", &N);

    // Allocate memory for the vector dynamically
    int *vec = (int *)malloc(N * sizeof(int));

    if (vec == NULL) {
        printf("Memory allocation failed\n");
        return 1; // Exit if memory allocation fails
    }

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
    printf("Resulting vector : ");
    for (int i = 0; i < N; i++) {
        printf("%d ", vec[i]);
    }
    printf("\n");

    // Free the allocated memory
    free(vec);

    return 0;
}

