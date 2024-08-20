#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int count = 0;  // Number of items in the buffer
int in = 0;     // Index to place the next produced item
int out = 0;    // Index to take the next consumed item

// Function for the producer to produce an item
void produce(int item) {
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
    count++;
}

// Function for the consumer to consume an item
int consume() {
    int item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    count--;
    return item;
}

int main() {
    omp_set_num_threads(2); // Setting 2 threads: one for producer and one for consumer

    #pragma omp parallel shared(buffer, count, in, out)
    {
        int id = omp_get_thread_num();
        if (id == 0) {
            // Producer thread
            for (int i = 0; i < 20; i++) {
                while (count == BUFFER_SIZE) {
                    // Buffer is full; wait for the consumer to consume
                    #pragma omp flush(count)
                }

                #pragma omp critical
                {
                    if (count < BUFFER_SIZE) {
                        produce(i);
                        printf("Produced: %d\n", i);
                        #pragma omp flush(count, buffer)
                    }
                }
            }
        } else {
            // Consumer thread
            for (int i = 0; i < 20; i++) {
                while (count == 0) {
                    // Buffer is empty; wait for the producer to produce
                    #pragma omp flush(count)
                }

                #pragma omp critical
                {
                    if (count > 0) {
                        int item = consume();
                        printf("Consumed: %d\n", item);
                        #pragma omp flush(count, buffer)
                    }
                }
            }
        }
    }

    return 0;
}
