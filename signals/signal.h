#include <signal.h>
#include <sys/types.h>

/*
Wrapper of the kill system call, which sends a signal to a process identified by its process ID (pid).
It sleeps for 1 second before sending the signal to ensure that the process is ready to receive it.
*/ 
int kill_with_sleep(pid_t pid, int sig);