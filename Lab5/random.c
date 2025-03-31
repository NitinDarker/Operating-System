/*
struct proc {
    int id;         // Process Number
    int arrival;    // Arrival Time (in seconds)
    int burst;      // Execution Time (in seconds)
    int waiting;    // Waiting Time
    int response;   // Response Time
    int turnaround; // Turnaround Time
    int time;       // Counter for time (for Gantt chart)
    int deadline;   // Deadline Time (in seconds)
} typedef proc;
*/

#include "proc.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int getRandom(int min, int max) {
    return min + rand() % (max - min + 1);
}

void randomize(proc *procList, int procNum) {
    for (int i = 0; i < procNum; i++) {
        procList[i].id = i + 1;
        procList[i].arrival = getRandom(0, procNum);
        procList[i].burst = getRandom(1, procNum);
        procList[i].deadline = getRandom(0, procNum);

        // Will evaluate later
        procList[i].waiting = 0;
        procList[i].response = 0;
        procList[i].turnaround = 0;
        procList[i].time = 0;
    }
}