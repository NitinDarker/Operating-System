#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int fd;
    char data[100];

    mkfifo("newf", 0666);
    fd = open("newf", O_RDONLY);
    read(fd, data, sizeof(data));
    printf("message received: %s", data);
    close(fd);
    unlink("newf");
    return 0;
}