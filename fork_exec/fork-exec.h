
/*
Fork and execute a new process specified by the given path and arguments.
Fork() creates a new process by duplicating the calling process. The new process is referred to as the child process. 
The child process then replaces its memory space with a new program specified by the path and arguments using execv(). 
The parent process waits for the child process to finish execution and retrieves its exit status.

MACROS:
WIFEXITED(status): returns true if the child process terminated normally (by calling exit or returning from main).
WEXITSTATUS(status): returns the exit status of the child process. This macro should only be used if WIFEXITED(status) returned true.
WIFSIGNALED(status): returns true if the child process was terminated by a signal.
WTERMSIG(status): returns the signal number that caused the child process to terminate. This macro should only be used if WIFSIGNALED(status) returned true.
*/
void fork_exec(const char* path, char* const argv[]);