// Ian Esselburn
// OS Project 1 
// consumer.c
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "table.h"

int main() {
    // create or open shared memory
    int shared = shm_open("/Shared_memory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shared == -1) {
        perror("Error on shm_open()");
        return 1;
    }

    struct table *consumer;
    consumer = mmap(0, sizeof(struct table), PROT_READ | PROT_WRITE, MAP_SHARED, shared, 0);
    if (consumer == MAP_FAILED) {
        perror("Error on mmap()");
        return 1;
    }

    // consume items from the shared buffer
    int turn = 0;
    while ( turn < iterations) {
        sem_wait(&consumer->filled_spots);
        for (int i = 0; i < MAX_TABLE_SIZE; ++i) {
            int item = consumer->shared_buffer[i];
            printf("consumer removed %d\n", item);
        }
        for (int i = 0; i < MAX_TABLE_SIZE; ++i) {
            sem_post(&consumer->empty_spots);
        }
        usleep(500000);
        ++turn;
    }

    return 0;
}
