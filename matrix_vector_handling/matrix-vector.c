#include <stdlib.h>
#include "matrix-vector.h"
#include <sys/shm.h>

LocalMatrix create_local_matrix(size_t rows, size_t cols)
{
    int **matrix_ptr = malloc(rows * sizeof(int *));
    for (size_t i = 0; i < rows; i++)
    {
        matrix_ptr[i] = malloc(cols * sizeof(int));
    }
    LocalMatrix matrix = {
        .rows = rows,
        .cols = cols,
        .data = matrix_ptr};
    return matrix;
}

// Uses Unix System V shared memory to create a matrix
ShmemMatrix create_shmem_matrix(size_t rows, size_t cols)
{
    shared_memory_t shmem_data = get_shared_memory(rows * cols);
    ShmemMatrix shmem_matrix = {
        .rows = rows,
        .cols = cols,
        .shmem_data = shmem_data};
    return shmem_matrix;
}

LocalVector create_local_vector(size_t size)
{
    LocalVector vector = {
        .size = size,
        .data = malloc(size * sizeof(int))};
    return vector;
}

ShmemVector create_shmem_vector(size_t size)
{
    shared_memory_t shmem_data = get_shared_memory(size);
    ShmemVector shmem_vector = {
        .size = size,
        .data = shmem_data};
    return shmem_vector;
}