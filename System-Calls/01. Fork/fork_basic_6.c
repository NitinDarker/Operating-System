#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void doit(void) {
    pid_t pid;
    fork();
    fork();
    printf("Unix System Programming\n");
    return;
}

int main(void) {
    doit();
    printf("Unix System Programming\n");
    exit(0);
}
