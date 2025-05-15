#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(void) {
	char write_msg[BUFFER_SIZE] = "Greetings";
	char read_msg[BUFFER_SIZE];
	int fd[2];
	pid_t pid;

	int pipe_value = pipe(fd); // Creates an ordinary pipe
	
	printf("Output of pipe(fd) = %d\n", pipe_value);

	if (pipe_value == -1) {
		fprintf(stderr, "Pipe failed\n");
		return 1;
	}

	pid = fork(); // Forks a child process
	
	if(pid < 0) {
		fprintf(stderr, "Fork Failed\n");
		return 1;
	}

	if (pid > 0) { // Parent Process

        printf("Parent process is running...\n");
        
		close(fd[READ_END]);
		// Close the unused end of the pipe
		
        printf("write() system call: \n");
		write(fd[WRITE_END], write_msg, strlen(write_msg)+1);
		// Write to the pipe
		
		close(fd[WRITE_END]);
		// Close the write end of the pipe now that write_msg is written
        
        printf("Parent process terminated~!\n");
	}
	else { // Child Process
        
        printf("Child process is running...\n");
        
		close(fd[WRITE_END]);
		// Close the unused end of the pipe
        
        printf("read() system call: \n");
		read(fd[READ_END], read_msg, BUFFER_SIZE);
		// Read from the pipe
        
		printf("Read %s\n", read_msg);
		// Prints the read message
        
		close(fd[READ_END]);
        
        printf("Child process terminated~!\n");
	}
	return 0;
}
