#include <stdlib.h>
#include "../shared_memory/shmem.h"

typedef struct
{
    size_t rows;
    size_t cols;
    int **data;
} LocalMatrix;

typedef struct
{
    size_t rows;
    size_t cols;
    shared_memory_t shmem_data;
} ShmemMatrix;

LocalMatrix create_local_matrix(size_t rows, size_t cols);

// Uses Unix System V shared memory to create a matrix
ShmemMatrix create_shmem_matrix(size_t rows, size_t cols);

void print_static_matrix(LocalMatrix matrix);

void print_shmem_matrix(ShmemMatrix matrix);

typedef struct
{
    size_t size;
    int *data;
} LocalVector;

typedef struct
{
    size_t size;
    shared_memory_t data;
} ShmemVector;

LocalVector create_local_vector(size_t size);

ShmemVector create_shmem_vector(size_t size);

void print_local_vector(LocalVector vector);

void print_shmem_vector(ShmemVector vector);
