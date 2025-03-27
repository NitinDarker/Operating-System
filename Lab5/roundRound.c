#include <stdio.h>

/*
struct proc {
    int id;         // Process Number
    int arrival;    // Arrival Time (in seconds)
    int burst;      // Execution Time (in seconds)
    int waiting;    // Waiting Time
    int response;   // Response Time
    int turnaround; // Turnaround Time
    int time;       // Counter for time (for Gantt chart)
} typedef proc;
*/

struct queue {
	proc *data;
	int front;
	int back;

	void create(int procNum) {
		data = calloc(procNum, sizeof(proc));
		front = 0;
		back = 0;
	}

	void push(proc p) {
			
		

void rr(proc *processList, int procNum) {

