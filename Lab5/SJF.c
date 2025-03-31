#include "generateResult.h"
#include "proc.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Shortest Job First (SJF) Algorithm
 * Non-Preemptive
 * Assuming Arrival Time = 0 for all processes
 * Process with least burst time executes first
 * In case of a tie, FCFS (First-Come-First-Serve) is used.
 */

void sjf(proc *procList, int procNum) {

    // Sort based on Burst Time -> Selection Sort
    for (int i = 0; i < procNum; i++) {
        int least = i;
        for (int j = i + 1; j < procNum; j++) {
            if (procList[j].burst < procList[least].burst) {
                least = j;
            }
        }
        if (least != i) {
            proc temp = procList[i];
            procList[i] = procList[least];
            procList[least] = temp;
        }
    }

    // Neglecting Arrival Time of all the processes

    // Calculate times for first process
    procList[0].waiting = 0;
    procList[0].response = 0;
    procList[0].time = procList[0].burst;
    procList[0].turnaround = procList[0].burst;
    // procList[0].arrival = 0;

    // Calculate times for remaining processes
    for (int i = 1; i < procNum; i++) {
        proc *p = &procList[i];
        proc *prev = &procList[i - 1];

        p->waiting = prev->time;
        p->response = p->waiting;
        p->time = prev->time + p->burst;
        p->turnaround = p->time;
        // p->arrival = 0;
    }

    // Output the result in a text file
    generate_text_output(procList, procNum, "SJF");
}