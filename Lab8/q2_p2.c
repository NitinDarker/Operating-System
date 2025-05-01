#include <signal.h> 
#include <stdio.h>
#include <stdlib.h>    
#include <sys/types.h> 

int main(int argc, char *argv[]) {
    // Check if a process ID was provided as a command-line argument
    if (argc != 2) {
        printf("No command line arguments found!");
        return 0;
    }

    pid_t pid = atoi(argv[1]);

    printf("Sending SIGINT to process %d\n", pid);

    if (kill(pid, SIGINT) == -1) {
        perror("Error sending signal"); // Print an error message if kill fails
        return 1;                      
    }

    printf("SIGINT sent successfully\n");

    return 0; 
}
