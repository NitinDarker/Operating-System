#ifndef STORAGE_H
#define STORAGE_H

#include "filesystem_internal.h" // For BLOCK_SIZE, TOTAL_BLOCKS

// Read a block from the simulated storage file.
void read_block(int block_index, unsigned char *buffer);

// Write a block to the simulated storage file.
void write_block(int block_index, const unsigned char *data);

// Load the FAT from its file into memory.
void load_fat();

// Save the FAT from memory to its file.
void save_fat();

// Load the Metadata Table from its file into memory.
void load_metadata();

// Save the Metadata Table from memory to its file.
void save_metadata();

#endif // STORAGE_H
