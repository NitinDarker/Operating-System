#include "generateResult.h"
#include "proc.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Shortest Remaining Time Next (SRTN) Algorithm
 * Preemptive version of SJF with Arrival Time Consideration
 * Process with the least remaining burst time executes first
 * In case of a tie, FCFS is used.
 */

void srtn(proc *procList, int procNum) {
    int completed = 0, currentTime = 0, minIndex;
    int remaining[procNum];
    int isCompleted[procNum];

    for (int i = 0; i < procNum; i++) {
        remaining[i] = procList[i].burst; 
        procList[i].waiting = 0;
        procList[i].response = -1;
        isCompleted[i] = 0;
    }

    while (completed < procNum) {
        minIndex = -1;

        for (int i = 0; i < procNum; i++) {
            if (!isCompleted[i] && procList[i].arrival <= currentTime && remaining[i] > 0) {
                if (minIndex == -1 || remaining[i] < remaining[minIndex]) {
                    minIndex = i;
                }
            }
        }

        if (minIndex == -1) {
            int earliestArrival = __INT_MAX__;
            for (int i = 0; i < procNum; i++) {
                if (!isCompleted[i] && procList[i].arrival < earliestArrival) {
                    earliestArrival = procList[i].arrival;
                }
            }
            currentTime = earliestArrival;
            continue;
        }

        // If first execution, set response time
        if (procList[minIndex].response == -1) {
            procList[minIndex].response = currentTime - procList[minIndex].arrival;
        }

        remaining[minIndex]--;
        currentTime++;

        // If process is completed
        if (remaining[minIndex] == 0) {
            completed++;
            isCompleted[minIndex] = 1;
            procList[minIndex].turnaround = currentTime - procList[minIndex].arrival;
            procList[minIndex].waiting = procList[minIndex].turnaround - procList[minIndex].burst;
        }
    }

    // Output the result
    generate_text_output(procList, procNum, "SRTN");
}
