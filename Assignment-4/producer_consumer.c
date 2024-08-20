#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int count = 0;
int in = 0, out = 0;

void produce(int item) {
    // Place the item in the buffer
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
    count++;
}

int consume() {
    // Remove an item from the buffer
    int item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    count--;
    return item;
}

int main() {
    int num_items = 20;

    #pragma omp parallel sections
    {
        // Producer Section
        #pragma omp section
        {
            for (int i = 0; i < num_items; i++) {
                int item = i + 1;  // Example item to produce

                // Wait for space in the buffer
                while (1) {
                    #pragma omp critical
                    {
                        if (count < BUFFER_SIZE) {
                            produce(item);
                            printf("Produced: %d\n", item);
                            break;
                        }
                    }
                }
            }
        }

        // Consumer Section
        #pragma omp section
        {
            for (int i = 0; i < num_items; i++) {
                int item;

                // Wait for an item to consume
                while (1) {
                    #pragma omp critical
                    {
                        if (count > 0) {
                            item = consume();
                            printf("Consumed: %d\n", item);
                            break;
                        }
                    }
                }
            }
        }
    }

    return 0;
}
