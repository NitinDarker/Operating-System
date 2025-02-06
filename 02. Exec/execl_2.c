#include <stdio.h>  /* for printf() */
#include <stdlib.h> /* for perror() */
#include <sys/types.h>
#include <unistd.h> /* for fork() */

void main(void) {
    printf("Executing ls\n");

    execl("/bin/src", "ls", "-la", (char *)0);

    /* If excel returns back, the call has failed... */
    perror("execl could not run it");
    exit(1);
}
