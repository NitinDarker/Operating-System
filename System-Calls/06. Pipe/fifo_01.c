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

// Function to demonstrate named pipe (FIFO)
void demo_named_pipe() {
    pid_t pid;
    char write_msg[MESSAGE_SIZE] = "Hello from Writer (Named Pipe)!";
    char read_msg[MESSAGE_SIZE];
    int fd_writer, fd_reader;

    printf("--- Demonstrating Named Pipe (FIFO) ---\n");
    printf("FIFO Name: %s\n", FIFO_NAME);

    // 1. Create the FIFO (Named Pipe)
    //    Permissions 0666: read/write for owner, group, others
    //    mkfifo returns 0 on success, -1 on error.
    //    If it already exists (EEXIST), that's okay for this demo, we'll just use it.
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        if (errno != EEXIST) { // If error is not "File exists"
            perror("mkfifo failed");
            exit(EXIT_FAILURE);
        }
        printf("Named pipe '%s' already exists. Reusing it.\n", FIFO_NAME);
    } else {
        printf("Named pipe '%s' created successfully.\n", FIFO_NAME);
    }

    // 2. Fork a process to act as reader and writer
    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        // Clean up FIFO if we created it and fork failed
        unlink(FIFO_NAME); // Best effort cleanup
        exit(EXIT_FAILURE);
    }

    if (pid > 0) { // Parent process (will act as Writer)
        printf("Writer (Parent PID %d): Opening FIFO '%s' for writing...\n", getpid(), FIFO_NAME);
        // Open FIFO for writing. This will block until a reader opens the other end.
        fd_writer = open(FIFO_NAME, O_WRONLY);
        if (fd_writer == -1) {
            perror("Writer: open FIFO for writing failed");
            unlink(FIFO_NAME); // Attempt to clean up
            exit(EXIT_FAILURE);
        }

        printf("Writer (Parent PID %d): Writing to FIFO: '%s'\n", getpid(), write_msg);
        if (write(fd_writer, write_msg, strlen(write_msg) + 1) == -1) { // +1 for null terminator
            perror("Writer: write to FIFO failed");
            close(fd_writer);
            unlink(FIFO_NAME);
            exit(EXIT_FAILURE);
        }
        close(fd_writer); // Close FIFO after writing
        printf("Writer (Parent PID %d): Message sent. Waiting for reader...\n", getpid());

        wait(NULL); // Wait for child (reader) to terminate
        printf("Writer (Parent PID %d): Reader finished.\n", getpid());

    } else { // Child process (pid == 0) (will act as Reader)
        // Small delay to ensure writer might open first (not strictly necessary due to blocking open)
        // sleep(1);
        printf("Reader (Child PID %d): Opening FIFO '%s' for reading...\n", getpid(), FIFO_NAME);
        // Open FIFO for reading. This will block until a writer opens the other end.
        fd_reader = open(FIFO_NAME, O_RDONLY);
        if (fd_reader == -1) {
            perror("Reader: open FIFO for reading failed");
            exit(EXIT_FAILURE); // No need to unlink here, writer might still be using it
        }

        printf("Reader (Child PID %d): Reading from FIFO...\n", getpid());
        ssize_t bytes_read = read(fd_reader, read_msg, MESSAGE_SIZE);

        if (bytes_read == -1) {
            perror("Reader: read from FIFO failed");
            close(fd_reader);
            exit(EXIT_FAILURE);
        } else if (bytes_read == 0) {
            printf("Reader (Child PID %d): FIFO empty or writer closed (EOF).\n", getpid());
        } else {
            read_msg[bytes_read] = '\0'; // Ensure null termination
            printf("Reader (Child PID %d): Read from FIFO: '%s'\n", getpid(), read_msg);
        }
        close(fd_reader); // Close FIFO after reading
        printf("Reader (Child PID %d): Exiting.\n", getpid());
        exit(EXIT_SUCCESS);
    }

    // Parent (or the process that created it) should clean up the named pipe
    // This ensures it's done after both processes are finished with it.
    if (pid > 0) { // Only parent does the final unlink
        printf("Parent (PID %d): Unlinking named pipe '%s'.\n", getpid(), FIFO_NAME);
        if (unlink(FIFO_NAME) == -1) {
            perror("unlink FIFO failed");
            // Not exiting here as the core demo might have worked
        }
    }
    printf("--- Named Pipe Demonstration Complete ---\n\n");
}

int main() {
    demo_named_pipe();
    return EXIT_SUCCESS;
}