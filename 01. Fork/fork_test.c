#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        printf("Nanna munna bacchA\n");
    } else {
        printf("Bijli ka bill tera baap bharega\n");
    }
}