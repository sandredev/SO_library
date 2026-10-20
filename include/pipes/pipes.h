#include <stdlib.h>

int fd[2]; // fd[0] is the read end, fd[1] is the write end. This is the pipe

void create_pipe();

void read_from_pipe(int fd, char* buffer, size_t size);

void write_to_pipe(int fd, const char* buffer, size_t size);

void close_pipe(int fd);

void read_binary_from_pipe(int fd, void* buffer, size_t size);

void write_binary_to_pipe(int fd, const void* buffer, size_t size);
