#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Ensuring the child runs first -> using wait
void main(void) {
	pid_t pid = fork();
	int status;
	if (pid < 0) {
		perror("Fork failed");
	} else if (pid > 0) {
		// Parent process
		wait(&status);
	}
	printf("\npid = %d\n", pid);
	if (pid == 0) {
		printf("Hi! I am child process\n");
		printf("As you can see that, I am running before my parent\n");
		printf("I will now be calling exec...\n");
		execl("/bin/ls", "My ls", "-1a", (char *)0);
	}
	printf("Hello! I am the parent process\n");
	printf("I will be running after my child process because I waited for his termination\n");
	if (WIFEXITED(status)) {
		status = WEXITSTATUS(status);
		printf("Child Exit status = %d\n", status);
	}
	exit(0);
}
