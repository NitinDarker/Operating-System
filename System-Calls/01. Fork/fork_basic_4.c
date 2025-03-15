#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    // escape sequence characters clears the output buffer
    // Watch what happens when there is no \n at the end...
    int pid;
    printf("One\n");
    pid = fork();
    printf("Two\n");
}
