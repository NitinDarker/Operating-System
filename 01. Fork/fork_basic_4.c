#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    int pid;
    printf("One\n");
    pid = fork();
    printf("Two\n");
}