#include <stdio.h>
#include <omp.h>

void printSeries2() {
    for (int i = 2; i <= 20; i += 2) {
        printf("2: %d\n", i);
    }
}

void printSeries4() {
    for (int i = 4; i <= 40; i += 4) {
        printf("4: %d\n", i);
    }
}

int main() {
    // Set the number of threads
    int num_threads = 2;
    omp_set_num_threads(num_threads);

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        if (thread_id == 0) {
            printSeries2();
        } else if (thread_id == 1) {
            printSeries4();
        }
    }

    return 0;
}
