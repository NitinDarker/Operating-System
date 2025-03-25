#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <stdatomic.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHM_NAME "/fork_bomb_shm"
#define MAX_FORKS 1000

// volatile sig_atomic_t *fork_count;

void cleanup_and_exit(int sig) {
    printf("\n💀💀💀 KILLING ALL PROCESSES! 💀💀💀\n\n");
    shm_unlink(SHM_NAME);
    kill(0, SIGKILL); // Kill all processes in the group
}

int main() {
    // Open a shared memory object
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(int));

    // Map shared memory
    atomic_int *fork_count = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Initialize counter
    *fork_count = 0; 

    // Set signal handler
    signal(SIGUSR1, cleanup_and_exit);

    while (1) {
        if (*fork_count >= MAX_FORKS) {
            kill(getppid(), SIGUSR1);
            break;
        }

        pid_t pid = fork();
        if (pid == 0) { // Child process
            (*fork_count)++;
            printf("[PID: %d] Process created! Fork count: %d\n", getpid(), *fork_count);
            sleep(0.5);
        } else if (pid > 0) { // Parent process
            continue;
        } else {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
