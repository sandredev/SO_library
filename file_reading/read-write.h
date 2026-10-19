#include <stdio.h>
#include "../matrix_vector_handling/matrix-vector.h"

FILE* load_file(char *src);

char* read_file_fgets(FILE *file, bool reset_byte_indicator);

char* read_file_fread(FILE *file, bool reset_byte_indicator);

int write_on_file_puts(FILE *file, char *content, bool reset_byte_indicator);

int write_on_file_fprintf(FILE *file, bool reset_byte_indicator);

// This function is very algorithm-dependent, it should read the file using fscanf and return the content as a string
// It can only be used as reference, not for real implementation.
char* read_file_fscanf(FILE *file, bool reset_byte_indicator);

LocalMatrix load_local_matrix_from_file(FILE *file, bool reset_byte_indicator, bool is_first_number_size);

LocalVector load_local_vector_from_file(FILE *file, bool reset_byte_indicator, bool is_first_number_size);

ShmemMatrix load_shmem_matrix_from_file(FILE *file, bool reset_byte_indicator, bool is_first_number_size);

ShmemVector load_shmem_vector_from_file(FILE *file, bool reset_byte_indicator, bool is_first_number_size);

