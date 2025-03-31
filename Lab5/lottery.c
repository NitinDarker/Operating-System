#include "generateResult.h"
#include "proc.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Lottery Scheduling Algorithm
 * Randomly selects a process based on assigned tickets
 * Higher burst time = More tickets for fairness
 */

void lottery(proc *procList, int procNum) {
    int completed = 0, currentTime = 0;
    int isCompleted[procNum];
    srand(time(NULL));

    for (int i = 0; i < procNum; i++) {
        isCompleted[i] = 0;
    }

    while (completed < procNum) {
        int totalTickets = 0;
        int selected = -1;

        // Calculate total tickets of processes that have arrived
        for (int i = 0; i < procNum; i++) {
            if (!isCompleted[i] && procList[i].arrival <= currentTime) {
                totalTickets += procList[i].burst; // More burst time = More tickets
            }
        }

        // If no process has arrived, move to the earliest arrival
        if (totalTickets == 0) {
            int earliestArrival = __INT_MAX__;
            for (int i = 0; i < procNum; i++) {
                if (!isCompleted[i] && procList[i].arrival < earliestArrival) {
                    earliestArrival = procList[i].arrival;
                }
            }
            currentTime = earliestArrival;
            continue;
        }

        // Perform the lottery selection
        int lotteryNumber = rand() % totalTickets;
        int cumulativeTickets = 0;

        for (int i = 0; i < procNum; i++) {
            if (!isCompleted[i] && procList[i].arrival <= currentTime) {
                cumulativeTickets += procList[i].burst;
                if (lotteryNumber < cumulativeTickets) {
                    selected = i;
                    break;
                }
            }
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
    generate_text_output(procList, procNum, "Lottery");
}
