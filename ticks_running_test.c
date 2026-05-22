#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf(1, "Usage: %s <pid>\n", argv[0]);
        exit();
    }

    int pid = atoi(argv[1]);  
    printf(1, "Provided PID: %d\n", pid);
	printf(1,"Current Process PID: %d\n", getpid());
    int i;
    for (i = 0; i < 1e7; i++) {
        asm("nop");     }

    int burst_time = ticks_running(pid);
    if(burst_time < 0){
    	printf(1,"Process with pid: %d doesnt exist\n", pid);
    }
    else {printf(1, "Burst time for process %d: %d ticks\n", pid, burst_time);}

    exit();
}

