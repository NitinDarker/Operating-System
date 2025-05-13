#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO "/tmp/newf"

// Consumer
int main() {
    int fd;
    char data[100];

    // Create FIFO only if it doesn't already exist
    if (access(FIFO, F_OK) == -1) {
        if (mkfifo(FIFO, 0666) == -1) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
        printf("FIFO created: %s\n", FIFO);
    }
    printf("Waiting for a writer to send data...\n");
    while (1) {
        fd = open(FIFO, O_RDONLY);
        if (fd == -1) {
            perror("open");
            sleep(1);
            continue;
        }
        ssize_t bytesRead = read(fd, data, sizeof(data));
        if (bytesRead > 0) {
            printf("message received: %s\n", data);
            close(fd);
            break;
        } else {
            printf("No data received yet - Waiting again...\n");
            close(fd);
            sleep(1);
        }
    }
    unlink(FIFO);
    return 0;
}
