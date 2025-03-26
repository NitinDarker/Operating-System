#include "random.c"
#include <stdio.h>
#include <stdlib.h>

/*
 * First-Come, First Served Algorithm
 * Randomly Generated Arrival Time
 * Randomly Generated Execution/Burst Time
 * Non-Preemptive
 * Sort on the basis of Arrival Time
 */

struct proc {
    int id;         // Process Number
    int arrival;    // Arrival Time (in seconds)
    int burst;      // Execution Time (in seconds)
    int waiting;    // Waiting Time
    int response;   // Response Time
    int turnaround; // Turnaround Time
    int time;       // Counter for time (for Gantt chart)
} typedef proc;

void print(proc *arr, int n);

int fcfs(int procNum) {
    proc *procList = (proc *)calloc(procNum, sizeof(proc));

    for (int i = 0; i < procNum; i++) {
        procList[i].id = i + 1;
        procList[i].arrival = getRandom(0, procNum);
        procList[i].burst = getRandom(1, 2 * procNum);

        // Will evaluate later
        procList[i].waiting = 0;
        procList[i].response = 0;
        procList[i].turnaround = 0;
        procList[i].time = 0;
    }

    // Sort based on Arrival Time
    for (int i = 0; i < procNum; i++) {
        int least = i;
        for (int j = i + 1; j < procNum; j++) {
            if (procList[j].arrival < procList[least].arrival) {
                least = j;
            }
        }
        if (least != i) {
            proc temp = procList[i];
            procList[i] = procList[least];
            procList[least] = temp;
        }
    }

    // Iterate over all the processes
    for (int i = 0; i < procNum; i++) {
        proc *p = &procList[i];
        proc *prev = &procList[i-1];
        if (i == 0) {
            p->waiting = 0;
            p->response = 0;
            p->turnaround = p->burst;
            p->time = p->arrival + p->turnaround;
            continue;
        }
        prev->time - p->arrival > 0 ? (p->waiting = prev->time - p->arrival) : 0;
        p->response = p->waiting;
        p->time = prev->time + p->burst; // Time (in seconds) when the process completes
        p->turnaround = p->waiting + p->burst;
    }

    print(procList, procNum);
    free(procList);
}

// For experimental purposes:
void print(proc *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("ID: %d\nAr: %d\nBu: %d\n", arr[i].id, arr[i].arrival, arr[i].burst);
        printf("Ti: %d\nRe: %d\nWa: %d\nTu: %d\n\n", arr[i].time, arr[i].response, arr[i].waiting, arr[i].turnaround);
    }
}

int main() {
    fcfs(10);
}