#include "types.h"
#include "stat.h"
#include "user.h"

void simulate_work(int duration) {
    for (volatile int i = 0; i < duration * 1000000; i++) {
        asm("nop");
    }
}

int main() {
    printf(1, "Demonstrating SJF Scheduler with 6 child processes.\n");

    int num_children = 6;
    int start_time, end_time, pid, job_length;

    for (int i = 0; i < num_children; i++) {
        pid = fork();
        if (pid < 0) {
            printf(1, "Fork failed for child %d!\n", i);
            exit();
        } else if (pid == 0) {
            printf(1, "Child %d (PID: %d) starting.\n", i, getpid());

           
            start_time = uptime();
            simulate_work(i + 1);
            end_time = uptime();
            job_length = sjf_job_length(getpid());

            printf(1, "Child %d (PID: %d) finished. Execution Time: %d ticks, Predicted Job Length: %d\n",
                   i, getpid(), end_time - start_time, job_length);

            exit();
        }
    }
    for (int i = 0; i < num_children; i++) {
        wait();
    }

    printf(1, "All child processes have completed.\n");
    exit();
}
