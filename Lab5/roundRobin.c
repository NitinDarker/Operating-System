#include "generateResult.h"
#include "proc.h"
#include <stdio.h>
#include <stdlib.h>

// Queue Implementation
typedef struct Node {
    int index;
    struct Node *next;
} Node;

typedef struct {
    Node *front, *rear;
} Queue;

void enqueue(Queue *q, int index) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->index = index;
    newNode->next = NULL;
    if (!q->rear) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

int dequeue(Queue *q) {
    if (!q->front)
        return -1;
    Node *temp = q->front;
    int index = temp->index;
    q->front = q->front->next;
    if (!q->front)
        q->rear = NULL;
    free(temp);
    return index;
}

int isEmpty(Queue *q) {
    return q->front == NULL;
}

void round_robin(proc *procList, int procNum, int quantum) {
    int completed = 0, currentTime = 0;
    int isCompleted[procNum];
    Queue q = {NULL, NULL};

    for (int i = 0; i < procNum; i++) {
        procList[i].waiting = 0;
        procList[i].response = -1;
        isCompleted[i] = 0;
    }

    int nextArrival = 0;
    while (completed < procNum) {
        // Add processes to the queue as they arrive
        while (nextArrival < procNum && procList[nextArrival].arrival <= currentTime) {
            enqueue(&q, nextArrival);
            nextArrival++;
        }

        if (isEmpty(&q)) {
            currentTime = procList[nextArrival].arrival;
            continue;
        }

        int index = dequeue(&q);

        if (procList[index].response == -1) {
            procList[index].response = currentTime - procList[index].arrival;
        }

        int execTime = (procList[index].burst > quantum) ? quantum : procList[index].burst;
        procList[index].burst -= execTime;
        currentTime += execTime;

        while (nextArrival < procNum && procList[nextArrival].arrival <= currentTime) {
            enqueue(&q, nextArrival);
            nextArrival++;
        }

        if (procList[index].burst > 0) {
            enqueue(&q, index);
        } else {
            completed++;
            isCompleted[index] = 1;
            procList[index].turnaround = currentTime - procList[index].arrival;
            procList[index].waiting = procList[index].turnaround - procList[index].burst;
        }
    }

    // Output result
    generate_text_output(procList, procNum, "Round Robin");
}

void run_round_robin(proc *procList, int procNum) {
    round_robin(procList, procNum, 1);
    round_robin(procList, procNum, 5);
    round_robin(procList, procNum, 10);
}
