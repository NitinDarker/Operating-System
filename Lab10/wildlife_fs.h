#ifndef WILDLIFE_FS_H
#define WILDLIFE_FS_H

#include "wildlife_fs_config.h"
#include "wildlife_fs_types.h"
#include <stdio.h>

// --- File System Initialization ---

// Creates the base directory if it doesn't exist
int fs_init();


// --- Primitive File Operations ---

// Creates an empty file.
int fs_create_file(const char *filename);

// Deletes a file.
int fs_delete_file(const char *filename);

// Opens a file. Returns a FILE pointer or NULL on error.
FILE *fs_open_file(const char *filename, const char *mode);

// Closes an open file.
void fs_close_file(FILE *fp);

// Writes encrypted data to an open file.
// Returns number of bytes written, or -1 on error.
long fs_write_encrypted(FILE *fp, const void *data, size_t size, const char *key);

// Reads and decrypts data from an open file.
// Returns number of bytes read, or -1 on error.
long fs_read_decrypted(FILE *fp, void *buffer, size_t size, const char *key);


// --- High-Level Operations ---

// Stores a photo. Filename is constructed internally.
int fs_store_photo(const void *photo_data, size_t photo_size, const char *location_tag, const char *key, char *out_filename);

// Retrieves a photo.
int fs_retrieve_photo(const char *filename, char **photo_data_out, size_t *photo_size_out, const char *key);

// Stores a single sensor reading. Appends to a daily or hourly log file.
int fs_store_sensor_reading(const SensorReading *reading, const char *key, char *out_logfile);

// Retrieves all sensor readings from a specific log file.
int fs_retrieve_sensor_log(const char *log_filename, SensorReading **readings_array_out, int *num_readings_out, const char *key);


// --- Send/Receive Files ---

// "Sends" a file (copies it to a transfer area, encrypted)
int fs_send_file_simulation(const char *local_filename, const char *remote_filename_alias, const char *key);

// "Receives" a file (copies from transfer area, decrypts and stores locally)
int fs_receive_file_simulation(const char *remote_filename_alias, const char *local_filename_to_save, const char *key);

#endif // WILDLIFE_FS_H
