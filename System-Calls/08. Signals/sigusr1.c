#include <signal.h> // For signal, SIGUSR1
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h> // For wait
#include <unistd.h>   // For fork, getpid, getppid, pause, kill, sleep

volatile sig_atomic_t sigusr1_received_flag = 0;

// Signal handler function for SIGUSR1
void simple_sigusr1_handler(int signum) {
    if (signum == SIGUSR1) {
        // For demo purposes, we'll add a simple write.
        char msg[] = "Parent: SIGUSR1 received via signal()!\n";
        write(STDOUT_FILENO, msg, sizeof(msg) - 1); // write is async-signal-safe
        sigusr1_received_flag = 1;
    }
}

int main() {
    pid_t pid;

    printf("Parent (PID %d): Starting (using signal()).\n", getpid());

    // Setup the signal handler for SIGUSR1 in the parent using signal()
    // signal() returns SIG_ERR on error.
    if (signal(SIGUSR1, simple_sigusr1_handler) == SIG_ERR) {
        perror("Parent: signal() failed to set handler for SIGUSR1");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        printf("Child (PID %d): My parent is PID %d.\n", getpid(), getppid());
        sleep(1); // Give parent a moment to be ready (handler setup is quick)

        printf("Child (PID %d): Sending SIGUSR1 to parent (PID %d).\n", getpid(), getppid());
        if (kill(getppid(), SIGUSR1) == -1) {
            perror("Child: kill failed to send SIGUSR1");
        }
        printf("Child (PID %d): Signal sent. Exiting.\n", getpid());
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        printf("Parent (PID %d): Child (PID %d) created. Waiting for SIGUSR1 via pause()...\n", getpid(), pid);

        while (!sigusr1_received_flag) {
            pause(); // Suspend until any signal is caught
                     // WARNING: If SIGUSR1 arrived before this pause(), it would be missed.
        }
        printf("Parent (PID %d): Woke up. sigusr1_received_flag is %d.\n", getpid(), sigusr1_received_flag);
        printf("Parent (PID %d): Proceeding after signal.\n", getpid());

        wait(NULL);
        printf("Parent (PID %d): Child has exited. Parent exiting.\n", getpid());
    }

    return EXIT_SUCCESS;
}
