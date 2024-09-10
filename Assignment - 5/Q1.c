#include <stdio.h>
#include <omp.h>

int main() {
    // Array of family member names
    const char* family_members[] = {"Shreya", "Alice", "Bob", "Charlie"};
    int num_members = 4;

    // Parallel region with a specified number of threads equal to family members
    #pragma omp parallel num_threads(num_members)
    {
        int thread_id = omp_get_thread_num();  // Get the thread ID
        if (thread_id < num_members) {
            printf("Thread ID: %d, Name: %s\n", thread_id, family_members[thread_id]);
        }
    }

    return 0;
}
