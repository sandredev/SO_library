#include "../../include/pipes/pipes.h"
#include <stdio.h>
#include <unistd.h>

void create_pipe() {
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
}

void close_pipe(int fd) {
    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}



// All the functions from here require analysis and attention to ensure they are implemented correctly and efficiently.

void read_from_pipe(int fd, char* buffer, size_t size) {
    size_t bytes_read = read(fd, buffer, size);
    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
}

void write_to_pipe(int fd, const char* buffer, size_t size) {
    size_t bytes_written = write(fd, buffer, size);
    if (bytes_written == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
}

void read_binary_from_pipe(int fd, void* buffer, size_t size) {
    size_t bytes_read = read(fd, buffer, size);
    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
}

void write_binary_to_pipe(int fd, const void* buffer, size_t size) {
    size_t bytes_written = write(fd, buffer, size);
    if (bytes_written == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
}