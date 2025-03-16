#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void checkZombie(int pid) {
    // To check the status of a process with given PID
    // Executes command: cat /proc/pid/status | grep "State"

    char cmd[100];
    sprintf(cmd, "cat /proc/%d/status | grep State", pid); // Shell command
    system(cmd);                                           // Execute in shell
}

// Creating a child process and making it as zombie
void main(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork Failed!\n");
    } 
    else if (pid == 0) {
        // Child process
        printf("\nPID = %d\n", pid);
        printf("Hi! I am child process!\n");
        printf("I will be exiting now...\n\n");
        exit(0);
    } 
    else {
        // Parent process
        sleep(1);
        printf("Hello! I am the parent process!\n");
        printf("Checking the state of my child now...\n");
        printf("\nPID = %d\n", pid);
        checkZombie(pid);
    }
}
