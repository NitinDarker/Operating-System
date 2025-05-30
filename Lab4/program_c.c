#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// Program C
int main(int argc, char *argv[]) {
    printf("Program C started (PID: %d)\n", getpid());

    /* 
    For replacing program_c with program_a:
    printf("Program C replacing itself with Program A...\n");
    execl("./q3", "q3", NULL); 
    // This will start a never ending recursion of program A
    perror("execl failed")
    */

    printf("Program C finishing\n");
    return 0;
}
