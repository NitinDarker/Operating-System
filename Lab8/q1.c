#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

// Signal handler function for SIGFPE
void handle_sigfpe(int signum) {
    printf("Do not divide by zero\n");
    // Exit the program gracefully after handling the signal
    exit(0);
}

int main() {
    int numerator;
    int denominator;

    signal(SIGFPE, handle_sigfpe);

    numerator = 5;
    denominator = 0;

    printf("%d\n", numerator / denominator);

    return 0;
}
