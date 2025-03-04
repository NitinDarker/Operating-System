#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main(void) {
	pid_t pid = fork();
	int globe = 5;

	if (pid == 0) {
		// Child process
		globe = 10;
		printf("Child: %d\n", globe);
		printf("Address: %p\n", &globe);
		sleep(1);
	}
	else if (pid > 0) {
		// Parent process
		globe = 100;
		printf("Parent: %d\n", globe);
		printf("Address: %p\n", &globe);
		sleep(1);
	}
	else {
		printf("Fork failed!");
	}
	printf("Final Value: %d\n", globe);
	return 0;
}
