#include <stdio.h>  /* for printf() */
#include <stdlib.h> /* for perror() */
#include <sys/types.h>
#include <sys/wait.h> /* for wait() */
#include <unistd.h>   /* for fork() */

int fatal(char *s);
int main(void) {
    pid_t pid;

    switch (pid = fork()) {
    case -1:
        fatal("fork failed");
        break;
    case 0:
        /* child process calls exec */
        sleep(1);
        execl("/bin/ls", "ls", "-l", (char *)0);
        fatal("exec failed");
        break;
    default:
        /* parent process uses wait to suspend execution
         * until child process finishes */
        // wait((int *)0);
        printf("ls completed\n");
        printf("%d", pid);
        exit(0);
    }
}

int fatal(char *s) {
    perror(s);
    exit(1);
}