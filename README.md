# SO Library

Colección de funciones prehechas en C para el curso de **Sistemas Operativos** del profesor **Germán Sánchez Torres**. Su objetivo es servir como **material de apoyo** para talleres y parciales, evitando reescribir la misma lógica en cada ejercicio.

> ⚠️ **Estado: en desarrollo.** El repositorio aún no está finalizado. Algunas funciones son borradores o requieren revisión (ver [Estado del proyecto](#estado-del-proyecto)).

---

## Módulos

| Módulo | Archivos | Descripción |
| --- | --- | --- |
| `file_reading` | `read-write.c`, `read-write.h` | Apertura, lectura y escritura de archivos con `FILE*` (`fopen`, `fgets`, `fputs`, `fprintf`, `fscanf`). Incluye carga de matrices y vectores desde archivo. |
| `matrix_vector_handling` | `matrix-vector.c`, `matrix-vector.h` | Estructuras y utilidades para matrices (`LocalMatrix`, `ShmemMatrix`) y vectores (`LocalVector`, `ShmemVector`), tanto en memoria dinámica como en memoria compartida. |
| `fork_exec` | `fork-exec.c`, `fork-exec.h` | Envoltorio de `fork()` + `execv()` + `waitpid()` para crear y ejecutar procesos hijos, reportando su estado de salida. |
| `pipes` | `pipes.c`, `pipes.h` | Creación y uso de tuberías (pipes) para comunicación entre procesos, con lectura/escritura de texto y de datos binarios. |
| `shared_memory` | `shmem.c`, `shmem.h` | Reserva y liberación de memoria compartida System V (`shmget`, `shmat`, `shmdt`, `shmctl`) para enteros, encapsulada en la estructura `shared_memory_t`. |
| `signals` | `signal.c`, `signal.h` | Envoltorio de `kill()` que espera un instante antes de enviar la señal a un proceso. |

Además, `.vscode/tasks.json` define una tarea de compilación con `gcc` para el archivo activo.

---

## Requisitos

- Linux u otro sistema tipo Unix (se usan llamadas POSIX y System V).
- `gcc` (o cualquier compilador de C compatible).
- `make` no es necesario: la compilación es directa con `gcc`.

---

## Uso

Cada módulo se compila junto con el archivo que lo utiliza. Por ejemplo:

```bash
# Compilar un programa propio que use lectura de archivos y matrices/vectores
gcc -g mi_programa.c file_reading/read-write.c \
    matrix_vector_handling/matrix-vector.c shared_memory/shmem.c \
    -o mi_programa

# Compilar un programa que use fork + exec
gcc -g mi_programa.c fork_exec/fork-exec.c -o mi_programa

# Compilar un programa que use pipes o señales
gcc -g mi_programa.c pipes/pipes.c -o mi_programa
gcc -g mi_programa.c signals/signal.c -o mi_programa
```

> `file_reading/read-write.c` es un módulo, no un programa independiente: no define `main()`, así que debes compilarlo junto con tu propio archivo principal.

En tus archivos fuente incluye el encabezado correspondiente:

```c
#include "matrix_vector_handling/matrix-vector.h"
#include "shared_memory/shmem.h"
#include "pipes/pipes.h"
#include "signals/signal.h"
#include "fork_exec/fork-exec.h"
```

### Ejemplo rápido: memoria compartida

```c
#include "shared_memory/shmem.h"

shared_memory_t shm = get_shared_memory(10); // 10 enteros
shm.ptr[0] = 42;

// ... otros procesos pueden leer shm.ptr[0] si comparten el shmid ...

free_shared_memory(shm.ptr, shm.shmid);
```

---

## Estructura del repositorio

```
SO_library/
├── file_reading/
│   ├── hello_world.txt
│   ├── read-write.c
│   └── read-write.h
├── fork_exec/
│   ├── fork-exec.c
│   └── fork-exec.h
├── matrix_vector_handling/
│   ├── matrix-vector.c
│   └── matrix-vector.h
├── pipes/
│   ├── pipes.c
│   └── pipes.h
├── shared_memory/
│   ├── shmem.c
│   └── shmem.h
├── signals/
│   ├── signal.c
│   └── signal.h
├── .vscode/
│   └── tasks.json
└── README.md
```

---

## Estado del proyecto

Este repositorio **no está terminado**. Puntos conocidos a revisar o completar:

- `read_file_fscanf` en `file_reading/read-write.c` es un esbozo sin implementar.
- `read_file_fgets` contiene lógica de crecimiento de buffer que debe revisarse.
- `pipes/pipes.h` declara `fd` como variable global, lo que puede generar problemas de enlazado al incluirse en varios archivos.
- `load_shmem_vector_from_file` y otras funciones de carga aún requieren pruebas.
- Faltan ejemplos de uso completos y pruebas para varios módulos.

Las contribuciones y correcciones son bienvenidas.

---

## Aviso

Material con fines exclusivamente educativos, elaborado como apoyo para el curso de Sistemas Operativos. Respeta las normas académicas de tu institución y cita la fuente cuando corresponda.
