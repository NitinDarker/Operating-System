#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

// Testing the effects of output buffer

int main(void) {
	int var = 5;
	pid_t pid;
	printf(" Parent ");
	pid = fork();
	printf(" Parent+child ");
	if (pid == 0) {
		// Checking if child process has access to var
		printf("\n\n\nvar = %d\n", var);
	}
	return 0;
}
