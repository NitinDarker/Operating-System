#include <sys/types.h> // has pid_t
#include <unistd.h> // has fork()
#include <stdio.h> // has printf()
#include <stdlib.h> // has exit()

int main(void) {
    // pid is a variable
    // pid_t is a data type to store PID of a process
    pid_t pid;
    pid = fork();
    printf("%d\n", pid);
    exit(0);
}
