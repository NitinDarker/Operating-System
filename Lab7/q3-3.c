/*
 * trylock.c
 * Demonstrates attempting to lock a POSIX mutex without blocking using trylock.
 * Compile: gcc trylock.c -o trylock -pthread
 */
#include <errno.h> // For EBUSY
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pthread_mutex_t my_mutex;

    printf("Initializing mutex...\n");
    if (pthread_mutex_init(&my_mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        return EXIT_FAILURE;
    }
    printf("Mutex initialized.\n");

    // First, lock the mutex normally
    printf("Locking the mutex with pthread_mutex_lock...\n");
    if (pthread_mutex_lock(&my_mutex) != 0) {
        perror("Initial mutex lock failed");
        pthread_mutex_destroy(&my_mutex);
        return EXIT_FAILURE;
    }
    printf("Mutex locked.\n");

    // Now, try to lock it again using trylock
    printf("Attempting to lock the *already locked* mutex with pthread_mutex_trylock...\n");
    int trylock_status = pthread_mutex_trylock(&my_mutex);

    if (trylock_status == 0) {
        // This block should NOT be reached in this specific example
        // because the mutex is already held.
        printf("Trylock acquired the lock (unexpected in this scenario!).\n");
        // If it somehow succeeded, unlock it
        pthread_mutex_unlock(&my_mutex);
    } else if (trylock_status == EBUSY) {
        // EBUSY is the expected error when the mutex is already locked.
        printf("Trylock failed with EBUSY, as expected (mutex is busy).\n");
    } else {
        // Handle other potential errors from trylock
        perror("Trylock failed with an unexpected error");
        // Unlock the original lock before exiting
        pthread_mutex_unlock(&my_mutex);
        pthread_mutex_destroy(&my_mutex);
        return EXIT_FAILURE;
    }

    // Now, unlock the original lock
    printf("Unlocking the original lock...\n");
    if (pthread_mutex_unlock(&my_mutex) != 0) {
        perror("Mutex unlock failed");
        pthread_mutex_destroy(&my_mutex);
        return EXIT_FAILURE;
    }
    printf("Mutex unlocked.\n");

    // Trylock should succeed now that the mutex is free
    printf("Attempting trylock on the *unlocked* mutex...\n");
    trylock_status = pthread_mutex_trylock(&my_mutex);
    if (trylock_status == 0) {
        printf("Trylock succeeded on the unlocked mutex, as expected.\n");
        // Unlock the lock acquired by trylock
        printf("Unlocking the mutex acquired by trylock...\n");
        if (pthread_mutex_unlock(&my_mutex) != 0) {
            perror("Mutex unlock (after trylock success) failed");
            pthread_mutex_destroy(&my_mutex);
            return EXIT_FAILURE;
        }
        printf("Mutex unlocked.\n");
    } else {
        perror("Trylock failed unexpectedly on unlocked mutex");
        pthread_mutex_destroy(&my_mutex);
        return EXIT_FAILURE;
    }

    printf("Destroying mutex...\n");
    if (pthread_mutex_destroy(&my_mutex) != 0) {
        perror("Mutex destruction failed");
        return EXIT_FAILURE;
    }
    printf("Mutex destroyed.\n");

    return EXIT_SUCCESS;
}
