#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Comparator function for ascending order
int compare_asc(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Comparator function for descending order
int compare_desc(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}

int main() {
    int N;

    // Taking input for the size of the vectors
    printf("Enter the size of the vectors: ");
    scanf("%d", &N);

    int vector1[N], vector2[N];

    // Taking input for vector1
    printf("Enter elements of vector1:\n");
    for (int i = 0; i < N; i++) {
        scanf("%d", &vector1[i]);
    }

    // Taking input for vector2
    printf("Enter elements of vector2:\n");
    for (int i = 0; i < N; i++) {
        scanf("%d", &vector2[i]);
    }

    // Sorting vector1 in ascending order
    qsort(vector1, N, sizeof(int), compare_asc);

    // Sorting vector2 in descending order
    qsort(vector2, N, sizeof(int), compare_desc);

    int min_scalar_product = 0;

    // Calculating the minimum scalar product using OpenMP parallelization
    #pragma omp parallel for reduction(+:min_scalar_product)
    for (int i = 0; i < N; i++) {
        min_scalar_product += vector1[i] * vector2[i];
    }

    // Output the result
    printf("The minimum scalar product is: %d\n", min_scalar_product);

    return 0;
}
