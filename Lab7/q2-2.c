/*
 * lock_unlock.c
 * Demonstrates locking and unlocking a POSIX mutex to protect a critical section.
 * Compile: gcc lock_unlock.c -o lock_unlock -pthread
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep()

pthread_mutex_t my_mutex;
int shared_resource = 0; // A simple shared resource

// Simple function executed by the thread
void *thread_function(void *arg) {
    printf("Thread: Attempting to lock the mutex...\n");

    // Acquire the lock. If another thread holds it, this thread will block.
    int lock_status = pthread_mutex_lock(&my_mutex);
    if (lock_status != 0) {
        perror("Thread: Mutex lock failed");
        pthread_exit(NULL); // Exit thread on failure
    }

    printf("Thread: Mutex locked successfully.\n");

    // --- Critical Section Start ---
    printf("Thread: Inside critical section.\n");
    shared_resource++; // Modify the shared resource safely
    printf("Thread: Incremented shared_resource to %d.\n", shared_resource);
    sleep(1); // Simulate some work
    printf("Thread: Leaving critical section.\n");
    // --- Critical Section End ---

    printf("Thread: Attempting to unlock the mutex...\n");

    // Release the lock so other threads can acquire it.
    int unlock_status = pthread_mutex_unlock(&my_mutex);
    if (unlock_status != 0) {
        perror("Thread: Mutex unlock failed");
        // Exiting without unlock might deadlock others, but handle error
        pthread_exit(NULL);
    }

    printf("Thread: Mutex unlocked successfully.\n");
    return NULL;
}

int main() {
    pthread_t tid; // Thread ID

    printf("Main: Initializing mutex...\n");
    if (pthread_mutex_init(&my_mutex, NULL) != 0) {
        perror("Main: Mutex initialization failed");
        return EXIT_FAILURE;
    }
    printf("Main: Mutex initialized.\n");

    printf("Main: Creating thread...\n");
    if (pthread_create(&tid, NULL, thread_function, NULL) != 0) {
        perror("Main: Thread creation failed");
        pthread_mutex_destroy(&my_mutex); // Clean up mutex
        return EXIT_FAILURE;
    }
    printf("Main: Thread created.\n");

    // Give the thread a chance to run and try to lock
    sleep(1); // Not strictly necessary but helps visualize

    // Main thread can also lock/unlock if needed, demonstrating contention
    printf("Main: Attempting to lock the mutex (will wait if thread holds it)...\n");
    if (pthread_mutex_lock(&my_mutex) != 0) {
        perror("Main: Mutex lock failed");
        // Fall through to cleanup
    } else {
        printf("Main: Mutex locked successfully.\n");
        printf("Main: Inside critical section.\n");
        shared_resource++;
        printf("Main: Incremented shared_resource to %d.\n", shared_resource);
        printf("Main: Leaving critical section.\n");
        if (pthread_mutex_unlock(&my_mutex) != 0) {
            perror("Main: Mutex unlock failed");
            // Fall through to cleanup
        } else {
            printf("Main: Mutex unlocked successfully.\n");
        }
    }

    printf("Main: Waiting for thread to complete...\n");
    if (pthread_join(tid, NULL) != 0) {
        perror("Main: Thread join failed");
        // Fall through to cleanup
    }
    printf("Main: Thread joined.\n");

    printf("Main: Destroying mutex...\n");
    if (pthread_mutex_destroy(&my_mutex) != 0) {
        perror("Main: Mutex destruction failed");
        return EXIT_FAILURE;
    }
    printf("Main: Mutex destroyed.\n");

    printf("Main: Final shared_resource value: %d\n", shared_resource);

    return EXIT_SUCCESS;
}
