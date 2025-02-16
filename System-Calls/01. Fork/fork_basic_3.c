#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    fork();
    fork();
    fork();
    printf("Unix program\n");
}