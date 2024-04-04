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

int main(int argc, char* argv[]) {
    srand(time(NULL)); // random seed for project

    int shared = shm_open("/Shared_memory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shared == -1) {
        perror("Error on shm_open()");
        return 1;
    }

    if (ftruncate(shared, sizeof(struct table)) == -1) {
        perror("Error on ftruncate()");
        return 1;
    }

    struct table *producer;
    producer = mmap(0, sizeof(struct table), PROT_READ | PROT_WRITE, MAP_SHARED, shared, 0);
    if (producer == MAP_FAILED) {
        perror("Error on mmap()");
        return 1;
    }

    sem_init(&producer->empty_spots, 1, MAX_TABLE_SIZE);
    sem_init(&producer->filled_spots, 1, 0);

    while (1) {
        for (int i = 0; i < MAX_TABLE_SIZE; ++i) {
            sem_wait(&producer->empty_spots); // Wait for empty spot
        }
        for (int i = 0; i < MAX_TABLE_SIZE; ++i) {
            int item = rand() % 100 + 1;
            producer->shared_buffer[i] = item;
            printf("producer produced %d\n", item);
        }
        sem_post(&producer->filled_spots); // Signal buffer is filled
        usleep(500000); // Delay for 0.5 second
    }

    sem_destroy(&producer->empty_spots);
    sem_destroy(&producer->filled_spots);

    return 0;
}
