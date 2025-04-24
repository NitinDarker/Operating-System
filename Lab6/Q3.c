#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_EARLY 5
#define NUM_LATE 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int early_completed = 0;

void *thread_func(void *arg) {
    int type = *(int *)arg;
    free(arg);

    if (type == 0) { // Early thread
        printf("Early thread %lu starting\n", (unsigned long)pthread_self());

        sleep(1);

        pthread_mutex_lock(&mutex);
        early_completed++;
        if (early_completed == NUM_EARLY) {
            // All early threads are completed
            pthread_cond_broadcast(&cond);
        }
        pthread_mutex_unlock(&mutex);

        printf("Early thread %lu completed\n", (unsigned long)pthread_self());
    } else { // Late thread
        pthread_mutex_lock(&mutex);
        while (early_completed < NUM_EARLY) {
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);

        printf("Late thread %lu starting\n", (unsigned long)pthread_self());

        sleep(1);

        printf("Late thread %lu completed\n", (unsigned long)pthread_self());
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_EARLY + NUM_LATE];

    // Create all threads simultaneously
    for (int i = 0; i < NUM_EARLY + NUM_LATE; i++) {
        int *arg = malloc(sizeof(int));
        *arg = (i < NUM_EARLY) ? 0 : 1;
        pthread_create(&threads[i], NULL, thread_func, arg);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_EARLY + NUM_LATE; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads completed\n");
    return 0;
}
