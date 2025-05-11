#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("p1 started with PID: %d\n", getpid());
    // Infinite loop
    for (;;) {
        pause();
    }
    printf("p1 successfully completed!\n");
    return 0;
}
