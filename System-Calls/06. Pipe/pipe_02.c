#include <errno.h> // For errno
#include <fcntl.h> // For open flags (O_RDONLY, O_WRONLY)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  // For mkfifo mode constants
#include <sys/types.h> // For pid_t, mkfifo
#include <sys/wait.h>  // For wait
#include <unistd.h>    // For pipe, fork, read, write, close, unlink

#define MESSAGE_SIZE 100
#define FIFO_NAME "/tmp/my_named_pipe" // Path for the named pipe

// Function to demonstrate normal (unnamed) pipe
void demo_normal_pipe() {
    int fd[2]; // File descriptors for the pipe: fd[0] is read end, fd[1] is write end
    pid_t pid;
    char write_msg[MESSAGE_SIZE] = "Hello from Parent (Normal Pipe)!";
    char read_msg[MESSAGE_SIZE];

    printf("--- Demonstrating Normal (Unnamed) Pipe ---\n");

    // 1. Create the pipe
    if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    // 2. Fork a child process
    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) { // Parent process
        printf("Parent (PID %d): Closing read end of normal pipe (fd[0]).\n", getpid());
        close(fd[0]); // Parent doesn't need to read from this pipe

        printf("Parent (PID %d): Writing to normal pipe: '%s'\n", getpid(), write_msg);
        if (write(fd[1], write_msg, strlen(write_msg) + 1) == -1) { // +1 for null terminator
            perror("Parent: write to pipe failed");
            close(fd[1]);
            exit(EXIT_FAILURE);
        }
        close(fd[1]); // Close write end after writing

        wait(NULL); // Wait for child to terminate
        printf("Parent (PID %d): Child finished.\n", getpid());

    } else { // Child process (pid == 0)
        printf("Child (PID %d): Closing write end of normal pipe (fd[1]).\n", getpid());
        close(fd[1]); // Child doesn't need to write to this pipe

        printf("Child (PID %d): Reading from normal pipe...\n", getpid());
        ssize_t bytes_read = read(fd[0], read_msg, MESSAGE_SIZE);
        if (bytes_read == -1) {
            perror("Child: read from pipe failed");
            close(fd[0]);
            exit(EXIT_FAILURE);
        } else if (bytes_read == 0) {
            printf("Child (PID %d): Pipe empty (EOF).\n", getpid());
        } else {
            read_msg[bytes_read] = '\0'; // Ensure null termination if not already
            printf("Child (PID %d): Read from normal pipe: '%s'\n", getpid(), read_msg);
        }
        close(fd[0]); // Close read end after reading
        printf("Child (PID %d): Exiting.\n", getpid());
        exit(EXIT_SUCCESS);
    }
    printf("--- Normal Pipe Demonstration Complete ---\n\n");
}

int main() {
    demo_normal_pipe();
    return EXIT_SUCCESS;
}
