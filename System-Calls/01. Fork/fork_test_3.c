#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
	// Testing loops in fork
	for (int i = 0; i < 3; i++) {
		printf("i=%d\n",i);
		fork();
	}
	return 0;
}
