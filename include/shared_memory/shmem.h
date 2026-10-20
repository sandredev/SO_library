#include <sys/shm.h>

typedef struct {
    int shmid;
    int* ptr;
} shared_memory_t;

// Only works for integers, if you want to use other types, you need to change the type of the pointer and the size of the shared memory
shared_memory_t get_shared_memory(size_t size);

int free_shared_memory(int* ptr, int shmid);