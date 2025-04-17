#include "generateResult.h"
#include "proc.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Highest Response Ratio Next (HRRN) Algorithm
 * Non-Preemptive Scheduling with Arrival Time
 * Response Ratio = (Waiting Time + Burst Time) / Burst Time
 */

void hrrn(proc *procList, int procNum) {
    int completed = 0, currentTime = 0;
    int isCompleted[procNum];

    for (int i = 0; i < procNum; i++) {
        isCompleted[i] = 0;
    }

    while (completed < procNum) {
        int selected = -1;
        double maxResponseRatio = -1.0;

        for (int i = 0; i < procNum; i++) {
            if (!isCompleted[i] && procList[i].arrival <= currentTime) {
                int waitingTime = currentTime - procList[i].arrival;
                double responseRatio = (double)(waitingTime + procList[i].burst) / procList[i].burst;

                if (responseRatio > maxResponseRatio) {
                    maxResponseRatio = responseRatio;
                    selected = i;
                }
            }
        }

        // If no process is available yet, move to the earliest arrival time
        if (selected == -1) {
            int earliestArrival = __INT_MAX__;
            for (int i = 0; i < procNum; i++) {
                if (!isCompleted[i] && procList[i].arrival < earliestArrival) {
                    earliestArrival = procList[i].arrival;
                }
            }
            currentTime = earliestArrival;
            continue;
        }

        // Assign calculated times
        procList[selected].waiting = currentTime - procList[selected].arrival;
        procList[selected].response = procList[selected].waiting;
        procList[selected].time = currentTime + procList[selected].burst;
        procList[selected].turnaround = procList[selected].time - procList[selected].arrival;

        currentTime = procList[selected].time;
        isCompleted[selected] = 1;
        completed++;
    }

    // Output the result
    generate_text_output(procList, procNum, "HRRN");
}
