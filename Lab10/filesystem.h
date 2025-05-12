#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdbool.h>

// --- Configuration (Public) ---
#define MAX_FILENAME_LENGTH 16 // Maximum length for file names

// --- File Types ---
#define FILE_TYPE_READING 0
#define FILE_TYPE_PHOTO 1

// --- Primitive Operations ---

// Initialize the file system. Opens/creates storage files.
void filesystem_init();

// Close the file system storage files.
void filesystem_close_storage();

// Create a new file.
// Returns 0 on success, -1 on failure.
int filesystem_create(const char *filename, int file_type);

// Delete a file.
// Returns 0 on success, -1 on failure.
int filesystem_delete(const char *filename);

// Store data in a file (basic overwrite).
// Returns 0 on success, -1 on failure.
int filesystem_store(const char *filename, const unsigned char *data, int data_size);

// Retrieve data from a file.
// Returns the number of bytes read on success, -1 on failure.
int filesystem_retrieve(const char *filename, unsigned char *buffer, int buffer_size);

// Open a file.
// Returns a file handle (index into metadata_table) on success, -1 on failure.
int filesystem_open(const char *filename);

// Close a file.
// Returns 0 on success, -1 on failure.
int filesystem_close(int file_handle);

#endif // FILESYSTEM_H
