#include <stdio.h>  /* for printf() */
#include <stdlib.h> /* for perror() */
#include <sys/types.h>
#include <unistd.h> /* for fork() */

int main(void) {
    printf("Executing ls\n");
    execl("/bin/ls", "love", "-1a", (char *)0);

    /* If excel returns back, the call has failed... */
    perror("execl could not run ls");
    exit(1);
}
