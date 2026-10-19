#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../matrix_vector_handling/matrix-vector.h"
#define SIZE 1024

/*
FILE is a structure that contains information about a file,
such as its name, mode, and current position in the file.
It is defined in the stdio.h header file and is used by the standard I/O functions
to perform file operations. The structure is opaque, meaning that its implementation
details are hidden from the user. The FILE structure typically contains the following members: {
    int EOF_indicator;
    int error_indicator;
    int byte_indicator;
}
*/

/*
Modes:
- READ: r
- WRITE: w
- READ BINARY: rb
- WRITE BINARY: wb
- WRITE AT END: a
- READ/WRITE (create file if it doesn't exist): r+
- READ/WRITE (NULL if file doesn't exist): w+ wb+ a+ ab+
*/
// It always opens the file in read/write mode, if it doesn't exist it creates it
FILE *load_file(char *src)
{
    FILE *file = fopen(src, "r+");

    if (file == NULL)
    {
        file = fopen(src, "w+");
    }

    if (file == NULL)
    {
        perror("Error al abrir el archivo");
        return file;
    }
    return file;
}

/*
  fseek(fptr, offset, position) can be interchangeable with rewind(fptr)
  the key difference is that rewind() resets the EOF and
  error indicator, while fseek only resets EOF
*/
char *read_file_fgets(FILE *file, bool reset_byte_indicator)
{
    if (reset_byte_indicator)
        fseek(file, 0, SEEK_SET);
    size_t capacity = SIZE;
    char *content = malloc(capacity);
    char buffer[SIZE];
    size_t len = 0, buffer_len = 0;
    while (fgets(buffer, SIZE, file) != NULL)
    {
        buffer_len = strlen(buffer);
        if (len + buffer_len + 1 > capacity)
        {
            size_t new_capacity = capacity * 2;
            while (new_capacity < len + buffer_len + 1)
            {
                new_capacity *= 2;
            }
            char *temp = realloc(content, new_capacity);
            if (temp == NULL)
            {
                free(content);
                perror("Error reallocating memory");
                return NULL;
            }
            content = temp;
            memcpy(content + len, buffer, buffer_len + 1);
            len += buffer_len;
        }
        memcpy(content + len, buffer, buffer_len);
        len += buffer_len;
        content[len] = '\0';
        buffer_len += buffer_len;
        printf("%ld byte read\n", ftell(file));
    }
    return content;
}
int write_on_file_puts(FILE *file, char *content, bool reset_byte_indicator)
{
    if (reset_byte_indicator)
        fseek(file, 0, SEEK_SET);
    return fputs(content, file);
}
int write_on_file_fprintf(FILE *file, bool reset_byte_indicator)
{
    if (reset_byte_indicator)
        fseek(file, 0, SEEK_SET);
    return fprintf(file, "Hello world!\n");
}

// Unfinished function, it should read the file using fscanf and return the content as a string.
// Since this function is very algorithm-dependent, it should be implemented in the main program
char *read_file_fscanf(FILE *file, bool reset_byte_indicator)
{
    if (reset_byte_indicator)
        fseek(file, 0, SEEK_SET);
    char *content;
    return content;
}

/*
Load matrix from file, the first two numbers in the file must be the number of rows
and columns of the matrix, respectively. The rest of the numbers are the elements of the matrix.
The function returns a pointer to a dynamically allocated 2D array (matrix) and
sets the number of rows and columns through the provided pointers.
*/
// This function uses shared memory to create the matrix, so it can be accessed by other processes.
ShmemMatrix load_shmem_matrix_from_file(FILE *file, bool reset_byte_indicator)
{
    if (reset_byte_indicator)
        fseek(file, 0, SEEK_SET);
    size_t rows, cols;
    fscanf(file, "%zu %zu", &rows, &cols);
    ShmemMatrix matrix = create_shmem_matrix(rows, cols);
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            fscanf(file, "%d", &matrix.shmem_data.ptr[i * cols + j]);
        }
    }
    return matrix;
}

LocalMatrix load_static_matrix_from_file(FILE *file, bool reset_byte_indicator)
{
    if (reset_byte_indicator)
        fseek(file, 0, SEEK_SET);
    size_t rows, cols;
    fscanf(file, "%zu %zu", &rows, &cols);
    LocalMatrix matrix = create_local_matrix(rows, cols);
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            fscanf(file, "%d", &matrix.data[i][j]);
        }
    }
    return matrix;
}

// Load vector from file, the first number in the file must be the size of the vector.
LocalVector load_local_vector_from_file(FILE *file, bool reset_byte_indicator, bool is_first_number_size)
{
    if (reset_byte_indicator)
        fseek(file, 0, SEEK_SET);
    size_t size;
    if (is_first_number_size)
    {
        fscanf(file, "%zu", &size);
    }
    else
    {
        fseek(file, 0, SEEK_END);
        size = ftell(file) / sizeof(int);
        fseek(file, 0, SEEK_SET);
    }
    LocalVector vector = create_local_vector(size);
    for (size_t i = 0; i < size; i++)
    {
        fscanf(file, "%d", &vector.data[i]);
    }
    return vector;
}

ShmemVector load_shmem_vector_from_file(FILE *file, bool reset_byte_indicator, bool is_first_number_size)
{
    if (reset_byte_indicator)
        fseek(file, 0, SEEK_SET);
    size_t size;
    if (is_first_number_size)
    {
        fscanf(file, "%zu", &size);
    }
    else
    {
        fseek(file, 0, SEEK_END);
        size = ftell(file) / sizeof(int);
        fseek(file, 0, SEEK_SET);
    }
    ShmemVector vector = create_shmem_vector(size);
    for (size_t i = 0; i < size; i++)
    {
        fscanf(file, "%d", &vector.shmem_data.ptr[i]);
    }
    return vector;
}

ShmemVector load_shmem_vector_from_file_size(FILE *file, bool reset_byte_indicator, size_t size) {
    if (reset_byte_indicator)
        fseek(file, 0, SEEK_SET);
    size_t actual_size = size;
    ShmemVector vector = create_shmem_vector(size);
    for (size_t i = 0; i < size; i++)
    {
        fscanf(file, "%d", &vector.shmem_data.ptr[i]);
    }
    return vector;
}
