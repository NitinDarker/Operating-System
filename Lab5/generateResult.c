#include "generateResult.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Generate text-based output for All scheduling algorithms
 * Creates a table showing process details and metrics
 */
void generate_text_output(proc *arr, int n, char *algo) {

    // Open file in append mode
    FILE *file = fopen("schedule.txt", "a");
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
    fprintf(file, "%s Process Scheduling Table\n", algo);
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
    fprintf(file, "|   Time      | Average Value   |\n");
    fprintf(file, "|_____________|_________________|\n");
    fprintf(file, "| Waiting     | %-15.2f |\n", avg_waiting);
    fprintf(file, "| Response    | %-15.2f |\n", avg_response);
    fprintf(file, "| Turnaround  | %-15.2f |\n", avg_turnaround);
    fprintf(file, "|_____________|_________________|\n\n\n");

    fclose(file);
}