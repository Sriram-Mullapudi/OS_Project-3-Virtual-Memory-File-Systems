#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    printf(1, "Testing set_sched_priority and get_sched_priority syscalls.\n");
    printf(1, "The current process (PID: %d) has an initial priority of %d.\n", getpid(), get_sched_priority(getpid()));
    set_sched_priority(1);
    printf(1, "The priority of the current process (PID: %d) has been updated to %d.\n", getpid(), get_sched_priority(getpid()));
    
    exit();
}
