# SO_library

A small C library of Unix / System V IPC building blocks, written as a study aid for an
Operating Systems course. Each module wraps one kernel primitive behind a short,
error-checked function so it can be reused across exercises and assignments.

The code targets **Linux**, uses **C99+** (designated initializers, `stdbool.h`, `size_t`),
and depends only on the standard POSIX / System V headers — no external libraries.

---

## Structure

```
SO_library/
├── shared_memory/           System V shared memory (shmget / shmat / shmdt / shmctl)
│   ├── shmem.h
│   └── shmem.c
├── matrix_vector_handling/  Matrices and vectors, in plain heap or shared memory
│   ├── matrix-vector.h
│   └── matrix-vector.c
├── pipes/                   Named wrappers over an anonymous pipe
│   ├── pipes.h
│   └── pipes.c
├── fork_exec/               fork() + execv() + waitpid() in one call
│   ├── fork-exec.h
│   └── fork-exec.c
├── signals/                 kill() wrapper
│   ├── signal.h
│   └── signal.c
└── file_reading/            File I/O helpers, incl. loading matrices/vectors from a file
    ├── read-write.c         (standalone — contains its own main())
    └── hello_world.txt
```

---

## Modules

### `shared_memory` — System V shared memory

The central abstraction of the library. Because a segment must be referenced by its
**`shmid`** (to destroy it) *and* by its **attached address** (to use it), both are carried
together in a struct:

```c
typedef struct {
    int shmid;
    int* ptr;
} shared_memory_t;

shared_memory_t get_shared_memory(size_t size);   // size is in int units
int  free_shared_memory(int* ptr, int shmid);     // detach, then mark for removal
```

`get_shared_memory` creates a private segment (`IPC_PRIVATE`, `IPC_CREAT | 0666`) and
attaches it, exiting the program on failure. `free_shared_memory` calls `shmdt()` first and
then `shmctl(shmid, IPC_RMID, NULL)` — the segment is destroyed once the last process
detaches.

```c
shared_memory_t shm = get_shared_memory(1024);    // 1024 ints
shm.ptr[0] = 42;
// ... other processes with the same shmid can read shm.ptr[0] ...
free_shared_memory(shm.ptr, shm.shmid);
```

> The pointer type is fixed to `int*`. To share another type, change `ptr` in
> `shared_memory_t` and the `size * sizeof(int)` computation in `get_shared_memory`.

### `matrix_vector_handling` — matrices and vectors

Four containers, all with the same field names so they can be used interchangeably:

| Type          | Backing store          | Creator                    |
| ------------- | ---------------------- | -------------------------- |
| `LocalMatrix` | heap (`int**`)         | `create_local_matrix`      |
| `ShmemMatrix` | shared memory          | `create_shmem_matrix`      |
| `LocalVector` | heap (`int*`)          | `create_local_vector`      |
| `ShmemVector` | shared memory          | `create_shmem_vector`      |

```c
ShmemMatrix m = create_shmem_matrix(3, 4);   // 3×4 in shared memory
LocalVector v = create_local_vector(16);     // 16 ints on the heap
```

Shared-memory variants wrap a `shared_memory_t`, so the `shmid` needed to release the
segment is preserved (`.shmem_data` on matrices, `.data` on vectors).

### `pipes` — anonymous pipe wrappers

```c
int fd[2];   // fd[0] = read end, fd[1] = write end

void create_pipe(void);
void close_pipe(int fd);
void read_from_pipe (int fd, char* buffer, size_t size);
void write_to_pipe(int fd, const char* buffer, size_t size);
void read_binary_from_pipe (int fd, void* buffer, size_t size);
void write_binary_to_pipe(int fd, const void* buffer, size_t size);
```

Each call reports failures with `perror` and exits. The binary variants take `void*` so
structs can be sent across a pipe unchanged.

### `fork_exec` — fork, exec and wait in one call

```c
void fork_exec(const char* path, char* const argv[]);
```

The child is replaced by `path` via `execv()` (the parent is unchanged); the parent
`waitpid()`s and prints the child's exit status. On success the function never returns in
the child. `argv[0]` should conventionally be the program name, and the array must be
`NULL`-terminated.

### `signals` — delayed kill

```c
int kill_with_sleep(pid_t pid, int sig);
```

Waits briefly before signalling, so the target process has time to install its handlers or
reach the point where the signal is meaningful, then forwards `kill`'s return value.

### `file_reading` — file helpers

This directory is a **program, not a header-based module** — `read-write.c` defines its own
`main()`.

```c
FILE* load_file(char* path);                  // "r+", falls back to "w+"

char* read_file_fgets(FILE* f, bool reset);   // whole file into a malloc'd string
int   write_on_file_puts(FILE* f, char* content, bool reset);
int   write_on_file_fprintf(FILE* f, bool reset);

ShmemMatrix load_shmem_matrix_from_file(FILE* f, bool reset);   // "rows cols" then values
LocalMatrix load_static_matrix_from_file(FILE* f, bool reset);
LocalVector load_local_vector_from_file(FILE* f, bool reset, bool is_first_number_size);
```

The `reset` flag rewinds the stream with `fseek(f, 0, SEEK_SET)` before reading or writing.
Matrix loaders expect the first two numbers to be the row and column counts; for vectors,
set `is_first_number_size` to `false` to derive the length from the file size instead of
reading it.

---

## Building

Every module compiles on its own:

```sh
gcc -Wall -Wextra -c shared_memory/shmem.c -o shmem.o
```

To build the runnable example in `file_reading/`, link it together with its dependencies:

```sh
gcc -Wall -Wextra \
    file_reading/read-write.c \
    matrix_vector_handling/matrix-vector.c \
    shared_memory/shmem.c \
    -o file_reading/read-write

./file_reading/read-write
```

The VS Code task in `.vscode/tasks.json` (`C/C++: gcc build active file`) compiles the file
currently open in the editor, which is convenient while stepping through a single module.

---

## Usage example

```c
#include "matrix_vector_handling/matrix-vector.h"

int main(void) {
    ShmemMatrix m = create_shmem_matrix(2, 2);

    m.shmem_data.ptr[0] = 1;
    m.shmem_data.ptr[1] = 2;
    m.shmem_data.ptr[2] = 3;
    m.shmem_data.ptr[3] = 4;

    free_shared_memory(m.shmem_data.ptr, m.shmem_data.shmid);
    return 0;
}
```

---

## Known limitations

Documented here so the current state is clear — none of these are hidden:

- **Headers have no include guards** (`#ifndef` / `#pragma once`), so including one twice
  from the same translation unit will break the build.
- `print_static_matrix`, `print_shmem_matrix`, `print_local_vector` and
  `print_shmem_vector` are declared in `matrix-vector.h` but not yet implemented.
- `pipes.h` *defines* `int fd[2]` rather than declaring it `extern`, so every file that
  includes it gets its own copy of the array.
- `pipes.c` stores the read/write result in a `size_t` and compares it against `-1`, so
  `-Wsign-compare` warns four times. `read()`/`write()` return `ssize_t`; storing in a
  signed type would silence them.
- `read_file_fscanf` is a stub: it returns an uninitialized pointer.
- Error handling is intentionally blunt — most failures `perror` and `exit()`, which is
  convenient in coursework programs but unsuitable for a library linked into a long-lived
  process.
- `signals/signal.c` calls `usleep(100)`, i.e. **100 microseconds**, while
  `signals/signal.h` documents it as a 1-second sleep. The two disagree.