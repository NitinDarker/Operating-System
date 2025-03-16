#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Ensuring the parent runs first -> using sleep
void main(void) {
	pid_t pid = fork();
	if (pid < 0) {
		perror("Fork failed");
	}
	else if (pid == 0) {
		// Child process
		sleep(1);
	}
	printf("\npid = %d\n", pid);
	if (pid == 0) {
		printf("Hi! I am child process\n");
		printf("I am running after my parent\n");
		printf("Executing ls through exec...\n");
		execl("/bin/ls", "chotu", "-1a", (char *)0);
		perror("Exec failed!\n");
	}
	printf("Hello! I am the parent process\n");
	printf("I will be running first this time around\n");
	printf("Passing the handle to my child\n");
	exit(0);
}


