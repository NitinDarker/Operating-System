#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO "/tmp/newf"

// Producer
int main() {
    int fd;
    char mes[] = "Hello World\n";

    mkfifo(FIFO, 0666);
    fd = open(FIFO, O_WRONLY);
    write(fd, mes, sizeof(mes));
    printf("message sent\n");
    close(fd);

    return 0;
}
