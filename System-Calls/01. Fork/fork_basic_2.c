#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    // Create 2^2=4 processes (including the parent)
    fork();
    fork();
    printf("Unix program\n");
}
