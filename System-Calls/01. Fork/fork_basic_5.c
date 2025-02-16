#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void main(void) {
    pid_t pid; /*holds process-id in parent*/
    char answer[5];

    printf("Just One Process so far\n");
    pid = fork(); /* create new process */

    printf("\nPress Enter");
    gets(answer);

    if (pid == 0)
        printf("I am the child\n");
    else if (pid > 0)
        printf(" I am the parent, child has pid %d\n", pid);
    else
        printf("Fork returned error code, no child\n");
}
