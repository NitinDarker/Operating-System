#include <stdio.h>  /* for printf() */
#include <stdlib.h> /* for perror() */
#include <sys/types.h>
#include <unistd.h> /* for fork() */

void main(void) {
    char *const av[] = {"ls", "-la", (char *)0};

    execv("/bin/ls", av);

    /* If execv returns back, the call has failed... */
    perror("execv could not run ls");
    exit(1);
}
// execv same as execl just 2nd to last arguments are stored in a vector
// vector means constant array of strings
