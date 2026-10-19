#include "shmem.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

shared_memory_t get_shared_memory(size_t size) {
    int shmid = shmget(IPC_PRIVATE, size * sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    int* ptr = shmat(shmid, NULL, 0);
    if (ptr == (int*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    shared_memory_t shmem = {
        .shmid = shmid,
        .ptr = ptr
    };
    return shmem;
}

int free_shared_memory(int* ptr, int shmid) {
    if (shmdt(ptr) == -1) {
        perror("shmdt");
        return -1;
    }
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}