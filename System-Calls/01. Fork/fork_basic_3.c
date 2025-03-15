#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    // Creates 2^4=16 processes (including parent)
    fork();
    fork();
    fork();
    fork();
    printf("Unix program\n");
}
