#include "../../include/signals/signal.h"
#include <unistd.h>

int kill_with_sleep(pid_t pid, int sig) {
    usleep(100);
    return kill(pid, sig);
}