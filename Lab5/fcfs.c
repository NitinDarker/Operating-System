#include "fcfs.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * First-Come, First Served Algorithm
 * Non-Preemptive
 * Sort on the basis of Arrival Time
 */
int fcfs(proc *procList, int procNum) {
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
        proc *prev = &procList[i-1];

        // Set completion time based on previous process
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

    generate_text_output(procList, procNum);
    return 0;
}

/*
 * Generate text-based output for FCFS scheduling
 * Creates a table showing process details and metrics
 */
void generate_text_output(proc *arr, int n) {
    FILE *file = fopen("fcfs_schedule.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Calculate averages
    float avg_waiting = 0, avg_response = 0, avg_turnaround = 0;
    for (int i = 0; i < n; i++) {
        avg_waiting += arr[i].waiting;
        avg_response += arr[i].response;
        avg_turnaround += arr[i].turnaround;
    }
    avg_waiting /= n;
    avg_response /= n;
    avg_turnaround /= n;

    // Print process table
    fprintf(file, "FCFS Process Scheduling Table\n");
    fprintf(file, "_____________________________________________________________________________\n");
    fprintf(file, "| Proc ID | Arrival | Burst | Completion | Waiting | Response | Turnaround |\n");
    fprintf(file, "|_________|_________|_______|____________|_________|__________|____________|\n");

    for (int i = 0; i < n; i++) {
        fprintf(file, "| %-7d | %-7d | %-5d | %-10d | %-7d | %-8d | %-10d |\n",
                arr[i].id,
                arr[i].arrival,
                arr[i].burst,
                arr[i].time,
                arr[i].waiting,
                arr[i].response,
                arr[i].turnaround);
    }

    fprintf(file, "|_________|_________|_______|____________|_________|__________|____________|\n\n");

    // Print averages
    fprintf(file, "Average Times:\n");
    fprintf(file, "_________________________________\n");
    fprintf(file, "| Metric      | Average Value   |\n");
    fprintf(file, "|_____________|_________________|\n");
    fprintf(file, "| Waiting     | %-15.2f |\n", avg_waiting);
    fprintf(file, "| Response    | %-15.2f |\n", avg_response);
    fprintf(file, "| Turnaround  | %-15.2f |\n", avg_turnaround);
    fprintf(file, "|_____________|_________________|\n");

    fclose(file);
    printf("FCFS scheduling results have been written to fcfs_schedule.txt\n");
}

