#include "wildlife_fs.h"
#include "wildlife_fs_crypto.h"
#include <errno.h>    // For errno
#include <stdlib.h>   // For malloc, free
#include <string.h>   // For strcpy, strcat, strlen, memset
#include <sys/stat.h> // For mkdir (might need different includes for Windows: <direct.h>)

// Helper to construct full path
static void build_full_path(const char *base, const char *filename, char *full_path_out) {
    strcpy(full_path_out, base);
    strcat(full_path_out, "/");
    strcat(full_path_out, filename);
}

int fs_init() {
    // For POSIX systems. For Windows, use _mkdir or CreateDirectory
    int result = mkdir(FS_BASE_PATH, 0777);
    if (result == 0) {
        printf("Base directory '%s' created.\n", FS_BASE_PATH);
    } else if (errno == EEXIST) {
        printf("Base directory '%s' already exists.\n", FS_BASE_PATH);
    } else {
        perror("Error creating base directory");
        return -1;
    }

    result = mkdir(FS_TRANSFER_PATH, 0777);
    if (result == 0) {
        printf("Transfer directory '%s' created.\n", FS_TRANSFER_PATH);
    } else if (errno == EEXIST) {
        printf("Transfer directory '%s' already exists.\n", FS_TRANSFER_PATH);
    } else {
        perror("Error creating transfer directory");
        return -1;
    }
    return 0;
}

int fs_create_file(const char *filename) {
    char full_path[MAX_PATH_LEN];
    build_full_path(FS_BASE_PATH, filename, full_path);

    FILE *fp = fopen(full_path, "wb"); // Create empty binary file
    if (!fp) {
        perror("fs_create_file: fopen error");
        return -1;
    }
    fclose(fp);
    return 0;
}

int fs_delete_file(const char *filename) {
    char full_path[MAX_PATH_LEN];
    build_full_path(FS_BASE_PATH, filename, full_path);

    if (remove(full_path) != 0) {
        perror("fs_delete_file: remove error");
        return -1;
    }
    return 0;
}

FILE *fs_open_file(const char *filename, const char *mode) {
    char full_path[MAX_PATH_LEN];
    build_full_path(FS_BASE_PATH, filename, full_path);
    return fopen(full_path, mode);
}

void fs_close_file(FILE *fp) {
    if (fp) {
        fclose(fp);
    }
}

long fs_write_encrypted(FILE *fp, const void *data, size_t size, const char *key) {
    if (!fp || !data || size == 0)
        return -1;

    char *buffer = (char *)malloc(size);
    if (!buffer) {
        fprintf(stderr, "fs_write_encrypted: malloc failed\n");
        return -1;
    }
    memcpy(buffer, data, size); // Copy data to mutable buffer

    if (key) {
        xor_cipher(buffer, size, key);
    }

    long written = fwrite(buffer, 1, size, fp);
    free(buffer);

    if (written != (long)size) {
        perror("fs_write_encrypted: fwrite error");
        return -1; // Indicate an error more strongly
    }
    return written;
}

long fs_read_decrypted(FILE *fp, void *buffer, size_t size, const char *key) {
    if (!fp || !buffer || size == 0)
        return -1;

    long bytes_read = fread(buffer, 1, size, fp);
    if (bytes_read <= 0) { // Could be error or EOF
        if (feof(fp))
            return 0; // EOF is not an error here, just 0 bytes read
        perror("fs_read_decrypted: fread error");
        return -1;
    }

    if (key) {
        xor_cipher((char *)buffer, bytes_read, key); // Decrypt only the bytes read
    }
    return bytes_read;
}

// --- High-Level Ops ---

int fs_store_photo(const void *photo_data, size_t photo_size, const char *location_tag, const char *key, char *out_filename) {
    time_t now = time(NULL);
    // Sanitize location_tag for filename (replace non-alphanum with '_')
    char sane_location_tag[64];
    int j = 0;
    for (int i = 0; location_tag[i] != '\0' && j < sizeof(sane_location_tag) - 1; ++i) {
        if (isalnum(location_tag[i]) || location_tag[i] == '_' || location_tag[i] == '-') {
            sane_location_tag[j++] = location_tag[i];
        }
    }
    sane_location_tag[j] = '\0';

    sprintf(out_filename, "IMG_%ld_%s.jpg", (long)now, sane_location_tag);

    FILE *fp = fs_open_file(out_filename, "wb");
    if (!fp) {
        fprintf(stderr, "fs_store_photo: Could not open file %s\n", out_filename);
        return -1;
    }

    if (fs_write_encrypted(fp, photo_data, photo_size, key) != (long)photo_size) {
        fprintf(stderr, "fs_store_photo: Error writing to file %s\n", out_filename);
        fs_close_file(fp);
        // Optionally delete partially written file: fs_delete_file(out_filename);
        return -1;
    }

    fs_close_file(fp);
    printf("Stored photo: %s (%zu bytes)\n", out_filename, photo_size);
    return 0;
}

int fs_retrieve_photo(const char *filename, char **photo_data_out, size_t *photo_size_out, const char *key) {
    FILE *fp = fs_open_file(filename, "rb");
    if (!fp) {
        fprintf(stderr, "fs_retrieve_photo: Could not open file %s\n", filename);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (file_size <= 0) {
        fprintf(stderr, "fs_retrieve_photo: Invalid file size for %s\n", filename);
        fs_close_file(fp);
        return -1;
    }

    *photo_data_out = (char *)malloc(file_size);
    if (!*photo_data_out) {
        fprintf(stderr, "fs_retrieve_photo: Malloc failed for %s\n", filename);
        fs_close_file(fp);
        return -1;
    }

    long bytes_read = fs_read_decrypted(fp, *photo_data_out, file_size, key);
    fs_close_file(fp);

    if (bytes_read != file_size) {
        fprintf(stderr, "fs_retrieve_photo: Read error or incomplete read for %s\n", filename);
        free(*photo_data_out);
        *photo_data_out = NULL;
        return -1;
    }

    *photo_size_out = bytes_read;
    printf("Retrieved photo: %s (%zu bytes)\n", filename, *photo_size_out);
    return 0;
}

int fs_store_sensor_reading(const SensorReading *reading, const char *key, char *out_logfile) {
    // Group logs by day for simplicity
    struct tm *timeinfo;
    timeinfo = localtime(&reading->timestamp);
    sprintf(out_logfile, "LOG_%04d%02d%02d.dat", 1900 + timeinfo->tm_year, 1 + timeinfo->tm_mon, timeinfo->tm_mday);

    FILE *fp = fs_open_file(out_logfile, "ab"); // Append binary
    if (!fp) {
        fprintf(stderr, "fs_store_sensor_reading: Could not open log file %s\n", out_logfile);
        return -1;
    }

    // For simplicity, writing the binary struct. CSV would be more human-readable but larger.
    if (fs_write_encrypted(fp, reading, sizeof(SensorReading), key) != sizeof(SensorReading)) {
        fprintf(stderr, "fs_store_sensor_reading: Error writing to log %s\n", out_logfile);
        fs_close_file(fp);
        return -1;
    }

    fs_close_file(fp);
    printf("Stored sensor reading to: %s\n", out_logfile);
    return 0;
}

int fs_retrieve_sensor_log(const char *log_filename, SensorReading **readings_array_out, int *num_readings_out, const char *key) {
    FILE *fp = fs_open_file(log_filename, "rb");
    if (!fp) {
        fprintf(stderr, "fs_retrieve_sensor_log: Could not open log file %s\n", log_filename);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (file_size < 0 || file_size % sizeof(SensorReading) != 0) {
        fprintf(stderr, "fs_retrieve_sensor_log: Invalid file size or format for %s (%ld bytes)\n", log_filename, file_size);
        fs_close_file(fp);
        return -1;
    }
    if (file_size == 0) { // Empty log file
        *readings_array_out = NULL;
        *num_readings_out = 0;
        fs_close_file(fp);
        return 0;
    }

    *num_readings_out = file_size / sizeof(SensorReading);
    *readings_array_out = (SensorReading *)malloc(file_size); // file_size is already total bytes for all structs
    if (!*readings_array_out) {
        fprintf(stderr, "fs_retrieve_sensor_log: Malloc failed for %s\n", log_filename);
        fs_close_file(fp);
        return -1;
    }

    long bytes_read = fs_read_decrypted(fp, *readings_array_out, file_size, key);
    fs_close_file(fp);

    if (bytes_read != file_size) {
        fprintf(stderr, "fs_retrieve_sensor_log: Read error for %s\n", log_filename);
        free(*readings_array_out);
        *readings_array_out = NULL;
        *num_readings_out = 0;
        return -1;
    }
    printf("Retrieved %d sensor readings from: %s\n", *num_readings_out, log_filename);
    return 0;
}

// --- Send/Receive Simulation ---
// Helper for send/receive simulation
static int copy_file_processed(const char *src_full_path, const char *dest_full_path, const char *key, int encrypt_mode) {
    FILE *src_fp = fopen(src_full_path, "rb");
    if (!src_fp) {
        perror("copy_file_processed: fopen src failed");
        fprintf(stderr, "Source file: %s\n", src_full_path);
        return -1;
    }
    FILE *dest_fp = fopen(dest_full_path, "wb");
    if (!dest_fp) {
        perror("copy_file_processed: fopen dest failed");
        fprintf(stderr, "Destination file: %s\n", dest_full_path);
        fclose(src_fp);
        return -1;
    }

    char buffer[1024];
    size_t bytes_read;
    int success = 0;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src_fp)) > 0) {
        char temp_buffer[1024]; // temp buffer for processing
        memcpy(temp_buffer, buffer, bytes_read);

        if (key) {
            xor_cipher(temp_buffer, bytes_read, key); // Encrypt or Decrypt
        }

        if (fwrite(temp_buffer, 1, bytes_read, dest_fp) != bytes_read) {
            perror("copy_file_processed: fwrite failed");
            success = -1;
            break;
        }
    }
    if (ferror(src_fp)) {
        perror("copy_file_processed: fread failed");
        success = -1;
    }

    fclose(src_fp);
    fclose(dest_fp);
    return success;
}

int fs_send_file_simulation(const char *local_filename, const char *remote_filename_alias, const char *key) {
    char src_full_path[MAX_PATH_LEN];
    char dest_full_path[MAX_PATH_LEN];

    build_full_path(FS_BASE_PATH, local_filename, src_full_path);
    build_full_path(FS_TRANSFER_PATH, remote_filename_alias, dest_full_path);

    printf("Simulating SEND: %s (local) to %s (transfer area, %s)\n",
           local_filename, remote_filename_alias, key ? "encrypted" : "plain");

    // During send, we encrypt the file into the transfer area
    return copy_file_processed(src_full_path, dest_full_path, key, 1 /* encrypt */);
}

int fs_receive_file_simulation(const char *remote_filename_alias, const char *local_filename_to_save, const char *key) {
    char src_full_path[MAX_PATH_LEN];
    char dest_full_path[MAX_PATH_LEN];

    build_full_path(FS_TRANSFER_PATH, remote_filename_alias, src_full_path);
    build_full_path(FS_BASE_PATH, local_filename_to_save, dest_full_path);

    printf("Simulating RECEIVE: %s (transfer area) to %s (local, %s)\n",
           remote_filename_alias, local_filename_to_save, key ? "decrypted" : "plain");

    // During receive, we decrypt the file from the transfer area
    return copy_file_processed(src_full_path, dest_full_path, key, 0 /* decrypt */);
}
