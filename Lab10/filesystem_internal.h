#ifndef FILESYSTEM_INTERNAL_H
#define FILESYSTEM_INTERNAL_H

#include <stdbool.h>
#include <stdio.h> // For FILE

// --- Configuration (Internal) ---
#define TOTAL_BLOCKS 100 // Total number of storage blocks
#define BLOCK_SIZE 256   // Size of each block in bytes
#define MAX_FILES 10     // Maximum number of files

// --- File Names for Persistent Storage ---
#define STORAGE_FILE "storage.bin"
#define FAT_FILE "fat.bin"
#define METADATA_FILE "metadata.bin"

// --- Special FAT values ---
#define FAT_FREE -1        // Indicates a free block
#define FAT_END_OF_FILE -2 // Indicates the last block of a file

// --- Data Structures (Internal) ---

// File Allocation Table (FAT) - in-memory cache
extern int FAT[TOTAL_BLOCKS];

// File Metadata Table - in-memory cache
struct FileMetadata {
    char filename[16];
    int file_type;   // 0 for reading, 1 for photo
    int file_size;   // in bytes
    int first_block; // index of the first data block in FAT
    bool is_deleted; // Flag to mark a file as deleted
};
extern struct FileMetadata metadata_table[MAX_FILES];

// --- File Pointers for Persistent Storage (Internal) ---
extern FILE *storage_fp;
extern FILE *fat_fp;
extern FILE *metadata_fp;

// --- File System State (Internal) ---
extern bool is_initialized;

#endif // FILESYSTEM_INTERNAL_H
