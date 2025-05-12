#include "storage.h"
#include <stdio.h>
#include <string.h>

// --- File Pointers for Persistent Storage (Defined here) ---
FILE *storage_fp = NULL;
FILE *fat_fp = NULL;
FILE *metadata_fp = NULL;

// Read a block from the simulated storage file
void read_block(int block_index, unsigned char *buffer) {
    if (!storage_fp || block_index < 0 || block_index >= TOTAL_BLOCKS) {
        fprintf(stderr, "Error: Invalid storage file pointer or block index %d.\n", block_index);
        return;
    }
    // Seek to the beginning of the desired block
    fseek(storage_fp, (long)block_index * BLOCK_SIZE, SEEK_SET);
    // Read the block data
    fread(buffer, 1, BLOCK_SIZE, storage_fp);
}

// Write a block to the simulated storage file
void write_block(int block_index, const unsigned char *data) {
    if (!storage_fp || block_index < 0 || block_index >= TOTAL_BLOCKS) {
        fprintf(stderr, "Error: Invalid storage file pointer or block index %d.\n", block_index);
        return;
    }
    // Seek to the beginning of the desired block
    fseek(storage_fp, (long)block_index * BLOCK_SIZE, SEEK_SET);
    // Write the block data
    fwrite(data, 1, BLOCK_SIZE, storage_fp);
    fflush(storage_fp); // Ensure data is written to the file immediately
}

// Load the FAT from its file
void load_fat() {
    if (!fat_fp) {
        fprintf(stderr, "Error: FAT file pointer is NULL.\n");
        return;
    }
    fseek(fat_fp, 0, SEEK_SET);
    fread(FAT, sizeof(int), TOTAL_BLOCKS, fat_fp);
}

// Save the FAT to its file
void save_fat() {
    if (!fat_fp) {
        fprintf(stderr, "Error: FAT file pointer is NULL.\n");
        return;
    }
    fseek(fat_fp, 0, SEEK_SET);
    fwrite(FAT, sizeof(int), TOTAL_BLOCKS, fat_fp);
    fflush(fat_fp); // Ensure data is written to the file immediately
}

// Load the Metadata Table from its file
void load_metadata() {
    if (!metadata_fp) {
        fprintf(stderr, "Error: Metadata file pointer is NULL.\n");
        return;
    }
    fseek(metadata_fp, 0, SEEK_SET);
    fread(metadata_table, sizeof(struct FileMetadata), MAX_FILES, metadata_fp);
}

// Save the Metadata Table to its file
void save_metadata() {
    if (!metadata_fp) {
        fprintf(stderr, "Error: Metadata file pointer is NULL.\n");
        return;
    }
    fseek(metadata_fp, 0, SEEK_SET);
    fwrite(metadata_table, sizeof(struct FileMetadata), MAX_FILES, metadata_fp);
    fflush(metadata_fp); // Ensure data is written to the file immediately
}

// Close the file system storage files.
void filesystem_close_storage() {
    if (storage_fp) {
        fclose(storage_fp);
        storage_fp = NULL;
    }
    if (fat_fp) {
        fclose(fat_fp);
        fat_fp = NULL;
    }
    if (metadata_fp) {
        fclose(metadata_fp);
        metadata_fp = NULL;
    }
    printf("Filesystem storage closed.\n");
}
