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
    int id;      // Process Number
    int arrival; // Arrival Time (in seconds)
    int burst;   // Execution Time (in seconds)
} typedef proc;

int fcfs(int procNum) {
    proc *procList = (proc *)calloc(procNum, sizeof(proc));
    
    for (int i = 0; i < procNum; i++) {
        procList[i].id = i+1;
        procList[i].arrival = rand();
        procList[i].burst = rand();
    }

    




    free(procList);
}


// For experimental purposes:
int main() {
    fcfs(10);
}