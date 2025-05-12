#include "filesystem.h"
#include "filesystem_internal.h"
#include "helpers.h"
#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Primitive Operations ---

// Initialize the file system
void filesystem_init() {
    if (is_initialized) {
        printf("Filesystem already initialized.\n");
        return;
    }

    // Open or create the storage files
    storage_fp = fopen(STORAGE_FILE, "rb+"); // Read and write, binary
    fat_fp = fopen(FAT_FILE, "rb+");
    metadata_fp = fopen(METADATA_FILE, "rb+");

    bool needs_formatting = false;

    if (!storage_fp || !fat_fp || !metadata_fp) {
        printf("Storage files not found. Formatting filesystem...\n");
        needs_formatting = true;

        // Try to create new files
        storage_fp = fopen(STORAGE_FILE, "wb+"); // Write, read, binary (creates if not exists)
        fat_fp = fopen(FAT_FILE, "wb+");
        metadata_fp = fopen(METADATA_FILE, "wb+");

        if (!storage_fp || !fat_fp || !metadata_fp) {
            fprintf(stderr, "Error: Could not create storage files.\n");
            exit(EXIT_FAILURE); // Exit if we can't create necessary files
        }
    }

    if (needs_formatting) {
        // Initialize FAT: all blocks are free
        for (int i = 0; i < TOTAL_BLOCKS; ++i) {
            FAT[i] = FAT_FREE;
        }
        save_fat(); // Save the initial FAT to file

        // Initialize Metadata Table: all entries are deleted/free
        for (int i = 0; i < MAX_FILES; ++i) {
            metadata_table[i].is_deleted = true;
            metadata_table[i].filename[0] = '\0'; // Clear filename
        }
        save_metadata(); // Save the initial Metadata Table to file

        // Optionally, write zeros to the storage file to initialize it
        unsigned char zero_block[BLOCK_SIZE] = {0};
        for (int i = 0; i < TOTAL_BLOCKS; ++i) {
            write_block(i, zero_block);
        }

    } else {
        // Load existing data from files
        load_fat();
        load_metadata();
        printf("Filesystem loaded from existing files.\n");
    }

    is_initialized = true;
    printf("Filesystem initialized.\n");
}

// Create a new file
// Returns 0 on success, -1 on failure
int filesystem_create(const char *filename, int file_type) {
    if (!is_initialized) {
        printf("Filesystem not initialized.\n");
        return -1;
    }
    if (strlen(filename) >= MAX_FILENAME_LENGTH) {
        printf("Filename too long.\n");
        return -1;
    }
    if (find_file_metadata_index(filename) != -1) {
        printf("File '%s' already exists.\n", filename);
        return -1;
    }

    int metadata_index = find_free_metadata_entry();
    if (metadata_index == -1) {
        printf("Metadata table is full. Cannot create file '%s'.\n", filename);
        return -1;
    }

    // Create the new file entry
    strncpy(metadata_table[metadata_index].filename, filename, MAX_FILENAME_LENGTH - 1);
    metadata_table[metadata_index].filename[MAX_FILENAME_LENGTH - 1] = '\0'; // Ensure null termination
    metadata_table[metadata_index].file_type = file_type;
    metadata_table[metadata_index].file_size = 0;
    metadata_table[metadata_index].first_block = FAT_FREE; // No blocks allocated yet
    metadata_table[metadata_index].is_deleted = false;

    save_metadata(); // Save the updated metadata table

    printf("File '%s' created successfully.\n", filename);
    return 0; // Success
}

// Delete a file
// Returns 0 on success, -1 on failure
int filesystem_delete(const char *filename) {
    if (!is_initialized) {
        printf("Filesystem not initialized.\n");
        return -1;
    }

    int metadata_index = find_file_metadata_index(filename);
    if (metadata_index == -1) {
        printf("File '%s' not found.\n", filename);
        return -1;
    }

    // Mark metadata entry as deleted
    metadata_table[metadata_index].is_deleted = true;
    metadata_table[metadata_index].filename[0] = '\0'; // Clear filename

    // Free the allocated data blocks in FAT
    int current_block = metadata_table[metadata_index].first_block;
    while (current_block != FAT_FREE && current_block != FAT_END_OF_FILE) {
        int next_block = FAT[current_block];
        FAT[current_block] = FAT_FREE; // Mark block as free
        current_block = next_block;
    }

    save_metadata(); // Save the updated metadata table
    save_fat();      // Save the updated FAT

    printf("File '%s' deleted successfully.\n", filename);
    return 0; // Success
}

// Store data in a file (basic append or overwrite for simplicity)
// Returns 0 on success, -1 on failure
int filesystem_store(const char *filename, const unsigned char *data, int data_size) {
    if (!is_initialized) {
        printf("Filesystem not initialized.\n");
        return -1;
    }

    int metadata_index = find_file_metadata_index(filename);
    if (metadata_index == -1) {
        printf("File '%s' not found. Cannot store data.\n", filename);
        return -1;
    }

    // For this basic implementation, we'll just overwrite existing data
    // and reallocate blocks if needed. A real system would handle appending,
    // fragmentation, etc.

    // First, free any existing blocks associated with the file
    int current_block = metadata_table[metadata_index].first_block;
    while (current_block != FAT_FREE && current_block != FAT_END_OF_FILE) {
        int next_block = FAT[current_block];
        FAT[current_block] = FAT_FREE; // Mark block as free
        current_block = next_block;
    }
    metadata_table[metadata_index].first_block = FAT_FREE; // Reset first block

    // Allocate new blocks and write data
    int remaining_data_size = data_size;
    int current_data_offset = 0;
    int previous_block = FAT_FREE;
    int first_allocated_block = FAT_FREE;

    while (remaining_data_size > 0) {
        int free_block_index = find_free_block();
        if (free_block_index == -1) {
            printf("Error: Storage full. Cannot store all data for '%s'.\n", filename);
            // In a real system, you might need to clean up partially written data
            // before returning an error.
            return -1;
        }

        // Write data to the block
        int bytes_to_write = (remaining_data_size < BLOCK_SIZE) ? remaining_data_size : BLOCK_SIZE;
        write_block(free_block_index, data + current_data_offset);

        // Update FAT
        if (previous_block != FAT_FREE) {
            FAT[previous_block] = free_block_index;
        } else {
            first_allocated_block = free_block_index; // This is the first block for the file
        }
        FAT[free_block_index] = FAT_END_OF_FILE; // Assume this is the last block for now

        previous_block = free_block_index;
        current_data_offset += bytes_to_write;
        remaining_data_size -= bytes_to_write;
    }

    // Update metadata
    metadata_table[metadata_index].file_size = data_size;
    metadata_table[metadata_index].first_block = first_allocated_block;

    save_metadata(); // Save updated metadata
    save_fat();      // Save updated FAT

    printf("Data stored in file '%s' successfully. Size: %d bytes.\n", filename, data_size);
    return 0; // Success
}

// Retrieve data from a file
// Returns the number of bytes read on success, -1 on failure
int filesystem_retrieve(const char *filename, unsigned char *buffer, int buffer_size) {
    if (!is_initialized) {
        printf("Filesystem not initialized.\n");
        return -1;
    }

    int metadata_index = find_file_metadata_index(filename);
    if (metadata_index == -1) {
        printf("File '%s' not found. Cannot retrieve data.\n", filename);
        return -1;
    }

    if (metadata_table[metadata_index].file_size > buffer_size) {
        printf("Buffer too small to retrieve all data for '%s'.\n", filename);
        // You might choose to retrieve partial data or return an error
        // Returning error for this basic version
        return -1;
    }

    int current_block = metadata_table[metadata_index].first_block;
    int bytes_read = 0;
    int current_buffer_offset = 0;

    while (current_block != FAT_FREE && current_block != FAT_END_OF_FILE && bytes_read < metadata_table[metadata_index].file_size) {
        unsigned char block_buffer[BLOCK_SIZE];
        read_block(current_block, block_buffer);

        int bytes_to_copy = (metadata_table[metadata_index].file_size - bytes_read < BLOCK_SIZE) ? (metadata_table[metadata_index].file_size - bytes_read) : BLOCK_SIZE;

        memcpy(buffer + current_buffer_offset, block_buffer, bytes_to_copy);

        bytes_read += bytes_to_copy;
        current_buffer_offset += bytes_to_copy;
        current_block = FAT[current_block]; // Move to the next block
    }

    printf("Data retrieved from file '%s'. Read %d bytes.\n", filename, bytes_read);
    return bytes_read; // Return number of bytes read
}

// Open a file
// Returns a file handle (index into metadata_table) on success, -1 on failure
int filesystem_open(const char *filename) {
    if (!is_initialized) {
        printf("Filesystem not initialized.\n");
        return -1;
    }

    int metadata_index = find_file_metadata_index(filename);
    if (metadata_index == -1) {
        printf("File '%s' not found or deleted.\n", filename);
        return -1;
    }

    // In a more complex system, opening might involve loading more metadata
    // or setting up file pointers. For this basic version, the index is enough.
    printf("File '%s' opened. Handle: %d.\n", filename, metadata_index);
    return metadata_index; // Return the metadata index as the file handle
}

// Close a file
// Returns 0 on success, -1 on failure
int filesystem_close(int file_handle) {
    if (!is_initialized) {
        printf("Filesystem not initialized.\n");
        return -1;
    }
    if (file_handle < 0 || file_handle >= MAX_FILES || metadata_table[file_handle].is_deleted) {
        printf("Invalid file handle %d.\n", file_handle);
        return -1;
    }

    // In this basic system, closing doesn't do much as changes are saved immediately
    // in store/delete. In a system with caching/buffering, this would flush data.
    printf("File handle %d closed.\n", file_handle);
    return 0; // Success
}
