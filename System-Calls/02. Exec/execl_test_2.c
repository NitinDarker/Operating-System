#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
	printf("Executing new process\n");
	// Trying to create an error
	execl("/bin/love", "Nitin's ls", "-1a", NULL);
	perror("execl failed\n");
	return 0;
}
// Results:
// Any name works for this new process (second argument of execl)
// (form third argument onwards are command line arguments for that new process
// NULL == (char *)0 as the last argument of execl
