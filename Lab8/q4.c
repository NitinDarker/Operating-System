#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid;

    // Flush the standard output buffer before forking.
    fflush(stdout);

    pid = fork();
    if (pid != 0) {
        printf("First\n");
    } else {
        printf("Second\n");
    }

    return 0;
}
