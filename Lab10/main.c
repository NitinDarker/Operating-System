#include "wildlife_fs.h"
#include <ctype.h> // For isalnum
#include <stdio.h>
#include <stdlib.h> // For free
#include <string.h> // For strcmp, memset
#include <unistd.h> // For sleep

int main() {
    if (fs_init() != 0) {
        fprintf(stderr, "Failed to initialize file system.\n");
        return 1;
    }

    const char *my_key = ENCRYPTION_KEY;
    char generated_filename[MAX_FILENAME_LEN];
    char generated_log_filename[MAX_FILENAME_LEN];

    // --- Test Photo Storage and Retrieval ---
    printf("\n--- Photo Test ---\n");
    char photo_content[] = "This is a dummy JPEG content for wildlife_pic_A! It's not a real JPEG.";
    size_t photo_size = strlen(photo_content);

    if (fs_store_photo(photo_content, photo_size, "RiverBank_Area51", my_key, generated_filename) == 0) {
        printf("Stored photo as: %s\n", generated_filename);

        char *retrieved_photo_data = NULL;
        size_t retrieved_photo_size = 0;
        if (fs_retrieve_photo(generated_filename, &retrieved_photo_data, &retrieved_photo_size, my_key) == 0) {
            printf("Retrieved photo: %s, Size: %zu\n", generated_filename, retrieved_photo_size);
            if (retrieved_photo_size == photo_size && memcmp(photo_content, retrieved_photo_data, photo_size) == 0) {
                printf("Photo content MATCHES!\n");
            } else {
                printf("Photo content MISMATCH!\n");
            }
            printf("Retrieved content: \"%.*s\"\n", (int)retrieved_photo_size, retrieved_photo_data); // Print first few chars
            free(retrieved_photo_data);
        } else {
            fprintf(stderr, "Failed to retrieve photo %s\n", generated_filename);
        }
    } else {
        fprintf(stderr, "Failed to store photo.\n");
    }

    // --- Test Sensor Log Storage and Retrieval ---
    printf("\n--- Sensor Log Test ---\n");
    SensorReading reading1 = {time(NULL), 25.5f, 60.1f, 0.5f, 5000.0f, 110.234f, 422.1f};
    sleep(1); // Ensure different timestamp for next reading
    SensorReading reading2 = {time(NULL), 26.0f, 59.5f, 0.2f, 5200.0f, 110.234f, 422.1f};

    if (fs_store_sensor_reading(&reading1, my_key, generated_log_filename) == 0) {
        printf("Stored reading1 to log: %s\n", generated_log_filename);
    } else {
        fprintf(stderr, "Failed to store sensor reading 1.\n");
    }
    // Store to the same log file (will be auto-determined by date)
    char another_log_filename[MAX_FILENAME_LEN];
    if (fs_store_sensor_reading(&reading2, my_key, another_log_filename) == 0) {
        printf("Stored reading2 to log: %s\n", another_log_filename);
        if (strcmp(generated_log_filename, another_log_filename) != 0) {
            printf("Warning: sensor readings went to different log files! (%s vs %s)\n", generated_log_filename, another_log_filename);
        }
    } else {
        fprintf(stderr, "Failed to store sensor reading 2.\n");
    }

    SensorReading *retrieved_readings = NULL;
    int num_readings = 0;
    if (fs_retrieve_sensor_log(generated_log_filename, &retrieved_readings, &num_readings, my_key) == 0) {
        printf("Retrieved %d sensor readings from %s:\n", num_readings, generated_log_filename);
        for (int i = 0; i < num_readings; i++) {
            printf("  Reading %d: Time=%ld, Temp=%.1fC, Hum=%.1f%%, Spd=%.1fm/s, Light=%.0flux\n",
                   i + 1,
                   (long)retrieved_readings[i].timestamp,
                   retrieved_readings[i].temperature_celsius,
                   retrieved_readings[i].humidity_percent,
                   retrieved_readings[i].speed_mps,
                   retrieved_readings[i].light_lux);
        }
        // Basic check for the first reading (assuming order is preserved)
        if (num_readings > 0 && retrieved_readings[0].temperature_celsius == reading1.temperature_celsius) {
            printf("First sensor reading data seems correct.\n");
        }
        free(retrieved_readings);
    } else {
        fprintf(stderr, "Failed to retrieve sensor log %s\n", generated_log_filename);
    }

    // --- Test Delete ---
    printf("\n--- Deletion Test ---\n");
    char test_del_file[] = "todelete.txt";
    if (fs_create_file(test_del_file) == 0) {
        printf("Created %s for deletion test.\n", test_del_file);
        if (fs_delete_file(test_del_file) == 0) {
            printf("Successfully deleted %s.\n", test_del_file);
            FILE *fp_check = fs_open_file(test_del_file, "rb");
            if (fp_check) {
                printf("ERROR: Deleted file %s still exists!\n", test_del_file);
                fs_close_file(fp_check);
            } else {
                printf("Confirmed %s is deleted.\n", test_del_file);
            }
        } else {
            fprintf(stderr, "Failed to delete %s.\n", test_del_file);
        }
    } else {
        fprintf(stderr, "Failed to create %s for deletion test.\n", test_del_file);
    }

    // --- Test Send/Receive Simulation ---
    printf("\n--- Send/Receive Simulation Test ---\n");
    // Use the photo we stored earlier (generated_filename)
    char remote_alias[] = "photo_from_device1.jpg_encrypted";
    char received_filename[] = "received_photo_device1.jpg";

    if (strlen(generated_filename) > 0) { // Ensure a photo was actually stored
        if (fs_send_file_simulation(generated_filename, remote_alias, my_key) == 0) {
            printf("Simulated sending %s as %s\n", generated_filename, remote_alias);

            if (fs_receive_file_simulation(remote_alias, received_filename, my_key) == 0) {
                printf("Simulated receiving %s as %s\n", remote_alias, received_filename);

                // Verify received file content
                char *original_photo_data = NULL;
                size_t original_photo_size = 0;
                char *received_photo_data_check = NULL;
                size_t received_photo_size_check = 0;
                int match = 0;

                if (fs_retrieve_photo(generated_filename, &original_photo_data, &original_photo_size, NULL) == 0) {                // Retrieve original unencrypted
                    if (fs_retrieve_photo(received_filename, &received_photo_data_check, &received_photo_size_check, NULL) == 0) { // Retrieve received unencrypted
                        if (original_photo_size == received_photo_size_check &&
                            memcmp(original_photo_data, received_photo_data_check, original_photo_size) == 0) {
                            match = 1;
                            printf("SUCCESS: Content of sent and received file MATCHES!\n");
                        }
                    }
                }
                if (!match)
                    printf("ERROR: Content of sent and received file MISMATCH or error in retrieval!\n");
                if (original_photo_data)
                    free(original_photo_data);
                if (received_photo_data_check)
                    free(received_photo_data_check);

            } else {
                fprintf(stderr, "Failed to simulate receiving file.\n");
            }
        } else {
            fprintf(stderr, "Failed to simulate sending file %s.\n", generated_filename);
        }
    } else {
        printf("Skipping send/receive test as no photo was stored successfully.\n");
    }

    printf("\n--- File System Demo Complete ---\n");
    printf("Check directories: '%s' and '%s'\n", FS_BASE_PATH, FS_TRANSFER_PATH);

    return 0;
}
