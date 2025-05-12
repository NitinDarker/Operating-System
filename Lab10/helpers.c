#include "helpers.h"
#include <string.h>

// --- File System State (Defined here) ---
bool is_initialized = false;

// --- Data Structures (Defined here) ---
int FAT[TOTAL_BLOCKS];
struct FileMetadata metadata_table[MAX_FILES];

// Find the index of a free block in the FAT
int find_free_block() {
    for (int i = 0; i < TOTAL_BLOCKS; ++i) {
        if (FAT[i] == FAT_FREE) {
            return i;
        }
    }
    return -1; // No free blocks found
}

// Find the index of a file in the metadata table by filename
int find_file_metadata_index(const char *filename) {
    for (int i = 0; i < MAX_FILES; ++i) {
        if (!metadata_table[i].is_deleted && strcmp(metadata_table[i].filename, filename) == 0) {
            return i; // File found
        }
    }
    return -1; // File not found
}

// Find a free entry in the metadata table
int find_free_metadata_entry() {
    for (int i = 0; i < MAX_FILES; ++i) {
        if (metadata_table[i].is_deleted) { // We can reuse deleted entries
            return i;
        }
    }
    return -1; // No free metadata entries
}
