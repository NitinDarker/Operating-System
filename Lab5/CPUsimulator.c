#include "fcfs.c"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("\n\n------------------------------\n");
    printf("Welcome to the Nitin's CPU Scheduling Algorithm Simulator!\n\n");
    printf("Please select a scheduling Algorithm:\n");
    printf("(1) FCFS \n(2) RR \n(3) SJF \n(4) SRTN \n(5) HRRN \n(6) Lottery \n(7) EDF \n(8) Priority \n(9) All \n> ");

    int algo;
    scanf("%d", &algo);

    printf("\nEnter the number of processes in the system: ");
    int proc;
    scanf("%d", proc);

    printf("\n\nPlease Wait... Generating Schedules...\n");
    sleep(10);

    switch (algo) {
        // TODO
    }

    return 0;
}

/*

Utilization Matrices for:
a. CPU Utilization
b. Waiting time of each process and average waiting time
c. Response time of each process and average response time
d. Turn-around time of each process and average turn-around time

Randomly Generate:
1) Arrival Time
2) CPU Burst or Execution time
3) Deadline

Create a file process_data.tex which contains randomly generated data for given no. of processes.

Additional features.
1) Gantt charts
2) Comparison table at the end for all the algorithms.

*/