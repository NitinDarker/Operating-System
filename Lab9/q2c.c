#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    FILE *fp;
    char data[100];

    fp = popen("./q2p", "r");

    printf("reading using popen...\n");

    while (fgets(data, sizeof(data), fp) != NULL) {
        printf("%s", data); // Reads line by line
    }

    pclose(fp);
}