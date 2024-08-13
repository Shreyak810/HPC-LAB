#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int numPoints = 1000000;  // Number of random points to generate
    int count = 0;            // Points inside the quarter circle

    // Parallel region for Monte Carlo simulation
    #pragma omp parallel for reduction(+:count)
    for (int i = 0; i < numPoints; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            count++;
        }
    }

    // Calculate Pi
    double pi = 4.0 * (double)count / numPoints;
  
    // Print the results
    printf("Estimated value of Pi: %f\n", pi);
    return 0;
}

