#include "generateResult.h"
#include "proc.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * First-Come, First-Served (FCFS) Algorithm
 * Non-Preemptive
 * Sort on the basis of Arrival Time
 */

void fcfs(proc *procList, int procNum) {

    // Sort based on Arrival Time -> Selection Sort
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

    // Calculate times for first process
    procList[0].waiting = 0;
    procList[0].response = 0;
    procList[0].time = procList[0].arrival + procList[0].burst;
    procList[0].turnaround = procList[0].burst;

    // Calculate times for remaining processes
    for (int i = 1; i < procNum; i++) {
        proc *p = &procList[i];
        proc *prev = &procList[i - 1];

        if (p->arrival > prev->time) {
            p->time = p->arrival + p->burst;
            p->waiting = 0;
        } else {
            p->time = prev->time + p->burst;
            p->waiting = prev->time - p->arrival;
        }

        p->response = p->waiting;
        p->turnaround = p->time - p->arrival;
    }

    // Output the result in a text file
    generate_text_output(procList, procNum, "FCFS");
}