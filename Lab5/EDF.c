#include "generateResult.h"
#include "proc.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Earliest Deadline First (EDF) Algorithm
 * A dynamic scheduling algorithm where the process with the earliest deadline is executed first
 * If multiple processes have the same deadline, FCFS is used.
 */

void edf(proc *procList, int procNum) {
    int completed = 0, currentTime = 0, minIndex;
    int isCompleted[procNum];

    for (int i = 0; i < procNum; i++) {
        procList[i].waiting = 0;
        procList[i].response = -1;
        isCompleted[i] = 0;
    }

    while (completed < procNum) {
        minIndex = -1;

        // Find process with the earliest deadline that has arrived
        for (int i = 0; i < procNum; i++) {
            if (!isCompleted[i] && procList[i].arrival <= currentTime) {
                if (minIndex == -1 || procList[i].deadline < procList[minIndex].deadline) {
                    minIndex = i;
                }
            }
        }

        if (minIndex == -1) {
            // No process is available, move time to the next arriving process
            int earliestArrival = __INT_MAX__;
            for (int i = 0; i < procNum; i++) {
                if (!isCompleted[i] && procList[i].arrival < earliestArrival) {
                    earliestArrival = procList[i].arrival;
                }
            }
            currentTime = earliestArrival;
            continue;
        }

        if (procList[minIndex].response == -1) {
            procList[minIndex].response = currentTime - procList[minIndex].arrival;
        }

        procList[minIndex].burst--;
        currentTime++;

        // If process is completed
        if (procList[minIndex].burst == 0) {
            completed++;
            isCompleted[minIndex] = 1;
            procList[minIndex].turnaround = currentTime - procList[minIndex].arrival;
            procList[minIndex].waiting = procList[minIndex].turnaround - procList[minIndex].burst;
        }
    }

    // Output the result
    generate_text_output(procList, procNum, "EDF");
}
