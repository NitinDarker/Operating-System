// If not defined
#ifndef PROC_H
#define PROC_H

typedef struct proc {
    int id;         // Process Number
    int arrival;    // Arrival Time (in seconds)
    int burst;      // Execution Time (in seconds)
    int deadline;   // Deadline Time (in seconds)
    int waiting;    // Waiting Time
    int response;   // Response Time
    int turnaround; // Turnaround Time
    int time;       // Counter for completion time (for Gantt chart)
} proc;

#endif