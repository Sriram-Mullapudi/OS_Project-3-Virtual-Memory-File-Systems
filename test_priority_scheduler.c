class Process:
    def __init__(self, pid, burst_time, priority):
        self.pid = pid           # Process ID
        self.burst_time = burst_time  # Time needed for execution
        self.priority = priority  # Priority (Lower value -> Higher priority)
        self.waiting_time = 0     # Time the process waits in the ready queue
        self.turnaround_time = 0  # Total time from arrival to completion

def priority_scheduling(processes):
    # Sort processes based on their priority (lower value -> higher priority)
    processes.sort(key=lambda x: x.priority)

    total_waiting_time = 0
    total_turnaround_time = 0

    print(f"{'PID':<5} {'Burst Time':<12} {'Priority':<10} {'Waiting Time':<12} {'Turnaround Time':<15}")

    # Calculate waiting time and turnaround time for each process
    for i, process in enumerate(processes):
        if i == 0:
            process.waiting_time = 0  # First process has no waiting time
        else:
            process.waiting_time = processes[i - 1].waiting_time + processes[i - 1].burst_time
        
        process.turnaround_time = process.waiting_time + process.burst_time

        total_waiting_time += process.waiting_time
        total_turnaround_time += process.turnaround_time

        print(f"{process.pid:<5} {process.burst_time:<12} {process.priority:<10} {process.waiting_time:<12} {process.turnaround_time:<15}")

    n = len(processes)
    print(f"\nAverage Waiting Time: {total_waiting_time / n:.2f}")
    print(f"Average Turnaround Time: {total_turnaround_time / n:.2f}")

# Sample input: List of processes with (pid, burst_time, priority)
process_list = [
    Process(1, 10, 3),
    Process(2, 1, 1),
    Process(3, 2, 4),
    Process(4, 1, 5),
    Process(5, 5, 2)
]

# Execute the priority scheduling algorithm
priority_scheduling(process_list)

