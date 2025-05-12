#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int fd;
    char mes[] = "Hello World\n";

    mkfifo("newf", 0666);
    fd = open("newf", O_WRONLY);
    write(fd, mes, sizeof(mes));
    printf("message sent\n");
    close(fd);

    return 0;
}
