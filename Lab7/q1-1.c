/*
 * init_destroy.c
 * Demonstrates initializing and destroying a POSIX mutex.
 * Compile: gcc init_destroy.c -o init_destroy -pthread
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> // For EXIT_SUCCESS/EXIT_FAILURE

int main() {
    pthread_mutex_t my_mutex; // Declare the mutex variable

    printf("Attempting to initialize the mutex...\n");

    // Initialize the mutex with default attributes (passing NULL)
    int init_status = pthread_mutex_init(&my_mutex, NULL);
    if (init_status != 0) {
        perror("Mutex initialization failed");
        return EXIT_FAILURE;
    }

    printf("Mutex initialized successfully.\n");

    // --- Mutex is now ready to be used (lock/unlock) ---
    // (We won't use it in this simple example, just init/destroy)
    printf("Mutex is ready for use.\n");
    // --- ---

    printf("Attempting to destroy the mutex...\n");

    // Destroy the mutex to release resources
    int destroy_status = pthread_mutex_destroy(&my_mutex);
    if (destroy_status != 0) {
        perror("Mutex destruction failed");
        // Note: Destroying a locked mutex results in undefined behavior.
        // Ensure the mutex is unlocked before destroying.
        return EXIT_FAILURE;
    }

    printf("Mutex destroyed successfully.\n");

    return EXIT_SUCCESS;
}
