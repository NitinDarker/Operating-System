#include "./EDF.c"
#include "./HRRN.c"
#include "./Lottery.c"
#include "./SJF.c"
#include "./SRTN.c"
#include "./fcfs.c"
#include "./roundRobin.c"
#include "generateResult.h"
#include "proc.h"
#include "random.c"
#include <stdio.h>
#include <stdlib.h>

int getRandom(int min, int max);
void randomize(proc *procList, int procNum);
void fcfs(proc *procList, int procNum);
void sjf(proc *procList, int procNum);
void srtn(proc *procList, int procNum);
void round_robin(proc *procList, int procNum, int quantum);
void run_round_robin(proc *procList, int procNum);
void hrrn(proc *procList, int procNum);
void lottery(proc *procList, int procNum);
void edf(proc *procList, int procNum);

int main() {
    printf("\n\n------------------------------\n");
    printf("Welcome to the Nitin's CPU Scheduling Algorithm Simulator!\n\n");
    printf("Please select a scheduling Algorithm:\n");
    printf("(1) FCFS \n(2) SJF \n(3) SRTN \n(4) RR \n(5) HRRN \n(6) Lottery \n(7) EDF \n(8) All \n> ");

    int algo;
    scanf("%d", &algo);

    printf("\nEnter the number of processes in the system: ");
    int procNum;
    scanf("%d", &procNum);

    // Generate a list of processes
    proc *procList = (proc *)calloc(procNum, sizeof(proc));
    randomize(procList, procNum);

    printf("\n\nPlease Wait... Generating Schedules...\n\n");

    switch (algo) {
    case 1:
        fcfs(procList, procNum);
        break;
    case 2:
        sjf(procList, procNum);
        break;
    case 3:
        srtn(procList, procNum);
        break;
    case 4:
        run_round_robin(procList, procNum);
        break;
    case 5:
        hrrn(procList, procNum);
        break;
    case 6:
        lottery(procList, procNum);
        break;
    case 7:
        edf(procList, procNum);
        break;
    case 8:
        fcfs(procList, procNum);
        sjf(procList, procNum);
        srtn(procList, procNum);
        run_round_robin(procList, procNum);
        hrrn(procList, procNum);
        lottery(procList, procNum);
        edf(procList, procNum);
        break;
    default:
        break;
    }

    printf("All the results are stored in schedule.txt\n");
    return 0;
}