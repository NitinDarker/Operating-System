#include <stdio.h>
#include <pthread.h>

void printer() {
    printf("I do stuff\n");
}

int main() {
    pthread_t myThread;
    int created = pthread_create(&myThread, NULL, (void *)printer, (void *)3);
    printf("Thread created: %d\n", created);
    int joined = pthread_join(myThread, NULL);
    printf("Thread joined: %d\n", joined);
    return 0;
}