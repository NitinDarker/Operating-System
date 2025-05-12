#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int fd[2];
    pid_t pid;
    char datain[25] = "parent has executed\n";
    char dataout[25];

    pipe(fd);
    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid > 0) {
        printf("parent start execution\n");
        sleep(1);
        close(fd[0]);
        write(fd[1], datain, strlen(datain) + 1);
        close(fd[1]);
    } else {
        close(fd[1]);
        read(fd[0], dataout, sizeof(dataout));
        printf("%s", dataout);
        printf("child process starts\n");
        sleep(1);
        printf("child process ends\n");
    }
    return 0;
}