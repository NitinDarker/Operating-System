#include "fcfs.h"
#include "proc.h"
#include "random.c"
#include <stdio.h>
#include <stdlib.h>

// Function declarations from random.c
int getRandom(int min, int max);
void randomize(proc *procList, int procNum);

// Function declaration from fcfs.c
int fcfs(proc *procList, int procNum);

int main() {
    printf("\n\n------------------------------\n");
    printf("Welcome to the Nitin's CPU Scheduling Algorithm Simulator!\n\n");
    printf("Please select a scheduling Algorithm:\n");
    printf("(1) FCFS \n(2) RR \n(3) SJF \n(4) SRTN \n(5) HRRN \n(6) Lottery \n(7) EDF \n(8) Priority \n(9) All \n> ");

    int algo;
    scanf("%d", &algo);

    printf("\nEnter the number of processes in the system: ");
    int procNum;
    scanf("%d", &procNum);

    // Generate a list of processes
    proc *procList = (proc *)calloc(procNum, sizeof(proc));
    randomize(procList, procNum);

    fcfs(procList, procNum);

    // printf("\n\nPlease Wait... Generating Schedules...\n");
    // sleep(5);

    // switch (algo) {
    //     // TODO
    // }

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