#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    // escape sequence characters clears the output buffer
    int pid;
    printf("One");
    pid = fork();
    printf("Two");
}
