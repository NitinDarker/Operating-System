/*
 * attr.c
 * Demonstrates initializing and destroying mutex attributes,
 * and using them to initialize a mutex.
 * Compile: gcc attr.c -o attr -pthread
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pthread_mutexattr_t mutex_attributes; // Mutex attributes object
    pthread_mutex_t my_mutex;             // Mutex variable

    printf("Attempting to initialize mutex attributes...\n");

    // Initialize the attributes object
    int attr_init_status = pthread_mutexattr_init(&mutex_attributes);
    if (attr_init_status != 0) {
        perror("Mutex attributes initialization failed");
        return EXIT_FAILURE;
    }
    printf("Mutex attributes initialized successfully (with default values).\n");

    // --- Optional: Set specific attributes here ---
    // Example: Set mutex type to error checking
    // printf("Setting mutex type attribute to error checking...\n");
    // if (pthread_mutexattr_settype(&mutex_attributes, PTHREAD_MUTEX_ERRORCHECK) != 0) {
    //     perror("Failed to set mutex type attribute");
    //     pthread_mutexattr_destroy(&mutex_attributes); // Clean up attr object
    //     return EXIT_FAILURE;
    // }
    // printf("Mutex type attribute set.\n");
    // --- ---

    printf("Attempting to initialize the mutex using the attributes...\n");

    // Initialize the mutex using the configured attributes object
    int mutex_init_status = pthread_mutex_init(&my_mutex, &mutex_attributes);
    if (mutex_init_status != 0) {
        perror("Mutex initialization using attributes failed");
        pthread_mutexattr_destroy(&mutex_attributes); // Clean up attr object
        return EXIT_FAILURE;
    }
    printf("Mutex initialized successfully using attributes.\n");

    // --- Attributes object is no longer needed after mutex initialization ---
    // It's good practice to destroy it once done.
    printf("Attempting to destroy mutex attributes object...\n");
    int attr_destroy_status = pthread_mutexattr_destroy(&mutex_attributes);
    if (attr_destroy_status != 0) {
        perror("Mutex attributes destruction failed");
        // Continue to destroy the mutex itself if possible
    } else {
        printf("Mutex attributes object destroyed successfully.\n");
    }
    // --- ---

    // --- Mutex can now be used ---
    printf("Mutex is ready for use.\n");
    // Example: Lock and unlock it
    if (pthread_mutex_lock(&my_mutex) == 0) {
        printf("Mutex locked and unlocked successfully.\n");
        pthread_mutex_unlock(&my_mutex);
    } else {
        perror("Mutex lock/unlock failed");
    }
    // --- ---

    printf("Attempting to destroy the mutex...\n");
    if (pthread_mutex_destroy(&my_mutex) != 0) {
        perror("Mutex destruction failed");
        return EXIT_FAILURE;
    }
    printf("Mutex destroyed successfully.\n");

    return EXIT_SUCCESS;
}
