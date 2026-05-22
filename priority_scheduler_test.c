#include <stdio.h>

// Define Process struct instead of using class
struct Process {
    int pid;             // Process ID
    int burst_time;      // Time needed for execution
    int priority;        // Priority (Lower value -> Higher priority)
    int waiting_time;    // Time the process waits in the ready queue
    int turnaround_time; // Total time from arrival to completion
};

// Function to sort processes based on priority (lower value -> higher priority)
void sort_by_priority(struct Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[j].priority < processes[i].priority) {
                struct Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

// Calculate waiting time and turnaround time for each process
void calculate_times(struct Process processes[], int n) {
    processes[0].waiting_time = 0; // First process has no waiting time
    for (int i = 1; i < n; i++) {
        processes[i].waiting_time = processes[i - 1].waiting_time + processes[i - 1].burst_time;
    }
    for (int i = 0; i < n; i++) {
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
    }
}

// Sample usage of the priority scheduling algorithm
int main() {
    struct Process processes[] = {
        {1, 10, 2},
        {2, 1, 1},
        {3, 2, 4},
        {4, 1, 3}
    };
    int n = sizeof(processes) / sizeof(processes[0]);

    sort_by_priority(processes, n);
    calculate_times(processes, n);

    printf("PID\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].pid,
               processes[i].burst_time,
               processes[i].priority,
               processes[i].waiting_time,
               processes[i].turnaround_time);
    }

    return 0;
}

