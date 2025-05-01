#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// Signal handler for SIGINT
void handle_sigint(int signum) {
    printf("SIGINT received, but ignoring it.\n");
}

int main() {
    printf("p1 started with PID: %d\n", getpid());

    signal(SIGINT, handle_sigint);

    // Infinite loop
    for (;;) {
        pause();
    }
    return 0; 
}
