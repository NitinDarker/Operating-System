#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("p1 started with PID: %d\n", getpid());
    // Infinite loop
    for (;;) {
        pause();
    }
    return 0;
}
