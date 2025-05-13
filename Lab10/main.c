#include "wildlife_fs.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void print_section(const char *title) {
    printf("\n-------------------------------\n");
    printf("%s\n", title);
    printf("-------------------------------\n");
}

int main() {
    if (fs_init() != 0) {
        fprintf(stderr, "Failed to initialize file system.\n");
        return 1;
    }

    const char *my_key = ENCRYPTION_KEY;
    char generated_filename[MAX_FILENAME_LEN];
    char generated_log_filename[MAX_FILENAME_LEN];

    // --- Photo Storage and Retrieval ---
    print_section("PHOTO TEST");

    char photo_content[] = "This is a dummy JPEG content for wildlife_pic_A! It's not a real JPEG.";
    size_t photo_size = strlen(photo_content);

    if (fs_store_photo(photo_content, photo_size, "RiverBank_Area51", my_key, generated_filename) == 0) {
        printf("Stored photo as: %s\n", generated_filename);

        char *retrieved_photo_data = NULL;
        size_t retrieved_photo_size = 0;
        if (fs_retrieve_photo(generated_filename, &retrieved_photo_data, &retrieved_photo_size, my_key) == 0) {
            printf("Retrieved photo (%s), Size: %zu bytes\n", generated_filename, retrieved_photo_size);

            if (retrieved_photo_size == photo_size &&
                memcmp(photo_content, retrieved_photo_data, photo_size) == 0) {
                printf("Photo content matches.\n");
            } else {
                printf("Photo content mismatch.\n");
            }

            printf("Content preview: \"%.*s\"\n", (int)retrieved_photo_size, retrieved_photo_data);
            free(retrieved_photo_data);
        } else {
            fprintf(stderr, "Failed to retrieve photo: %s\n", generated_filename);
        }
    } else {
        fprintf(stderr, "Failed to store photo.\n");
    }

    // --- Sensor Log Storage and Retrieval ---
    print_section("SENSOR LOG TEST");

    SensorReading reading1 = {time(NULL), 25.5f, 60.1f, 0.5f, 5000.0f, 110.234f, 422.1f};
    sleep(1);
    SensorReading reading2 = {time(NULL), 26.0f, 59.5f, 0.2f, 5200.0f, 110.234f, 422.1f};

    if (fs_store_sensor_reading(&reading1, my_key, generated_log_filename) == 0) {
        printf("Stored sensor reading 1 to: %s\n", generated_log_filename);
    } else {
        fprintf(stderr, "Failed to store sensor reading 1.\n");
    }

    char another_log_filename[MAX_FILENAME_LEN];
    if (fs_store_sensor_reading(&reading2, my_key, another_log_filename) == 0) {
        printf("Stored sensor reading 2 to: %s\n", another_log_filename);
        if (strcmp(generated_log_filename, another_log_filename) != 0) {
            printf("Note: Sensor readings went to different log files.\n");
        }
    } else {
        fprintf(stderr, "Failed to store sensor reading 2.\n");
    }

    SensorReading *retrieved_readings = NULL;
    int num_readings = 0;

    if (fs_retrieve_sensor_log(generated_log_filename, &retrieved_readings, &num_readings, my_key) == 0) {
        printf("\nRetrieved %d reading(s) from %s:\n", num_readings, generated_log_filename);
        for (int i = 0; i < num_readings; i++) {
            printf("  Reading %d: Time=%ld, Temp=%.1fC, Hum=%.1f%%, Spd=%.1fm/s, Lux=%.0f\n",
                   i + 1,
                   (long)retrieved_readings[i].timestamp,
                   retrieved_readings[i].temperature_celsius,
                   retrieved_readings[i].humidity_percent,
                   retrieved_readings[i].speed_mps,
                   retrieved_readings[i].light_lux);
        }

        if (num_readings > 0 && retrieved_readings[0].temperature_celsius == reading1.temperature_celsius) {
            printf("First reading appears to be correct.\n");
        }

        free(retrieved_readings);
    } else {
        fprintf(stderr, "Failed to retrieve sensor log.\n");
    }

    // --- Deletion Test ---
    print_section("FILE DELETION TEST");

    char test_del_file[] = "todelete.txt";

    if (fs_create_file(test_del_file) == 0) {
        printf("Created file for deletion test: %s\n", test_del_file);

        if (fs_delete_file(test_del_file) == 0) {
            printf("Deleted file: %s\n", test_del_file);

            FILE *fp_check = fs_open_file(test_del_file, "rb");
            if (fp_check) {
                printf("WARNING: File still exists after deletion.\n");
                fs_close_file(fp_check);
            } else {
                printf("File deletion confirmed.\n");
            }
        } else {
            fprintf(stderr, "Failed to delete file: %s\n", test_del_file);
        }
    } else {
        fprintf(stderr, "Failed to create test file.\n");
    }

    // --- Send/Receive Simulation ---
    print_section("SEND/RECEIVE SIMULATION");

    char remote_alias[] = "photo_from_device1.jpg_encrypted";
    char received_filename[] = "received_photo_device1.jpg";

    if (strlen(generated_filename) > 0) {
        if (fs_send_file_simulation(generated_filename, remote_alias, my_key) == 0) {
            printf("Simulated sending: %s -> %s\n", generated_filename, remote_alias);

            if (fs_receive_file_simulation(remote_alias, received_filename, my_key) == 0) {
                printf("Simulated receiving: %s -> %s\n", remote_alias, received_filename);

                char *original_photo_data = NULL;
                size_t original_photo_size = 0;
                char *received_photo_data = NULL;
                size_t received_photo_size = 0;
                int match = 0;

                if (fs_retrieve_photo(generated_filename, &original_photo_data, &original_photo_size, NULL) == 0 &&
                    fs_retrieve_photo(received_filename, &received_photo_data, &received_photo_size, NULL) == 0) {

                    if (original_photo_size == received_photo_size &&
                        memcmp(original_photo_data, received_photo_data, original_photo_size) == 0) {
                        match = 1;
                        printf("SUCCESS: Sent and received files match.\n");
                    }
                }

                if (!match)
                    printf("ERROR: Sent and received photo contents differ.\n");

                free(original_photo_data);
                free(received_photo_data);
            } else {
                fprintf(stderr, "Failed to simulate receiving.\n");
            }
        } else {
            fprintf(stderr, "Failed to simulate sending file.\n");
        }
    } else {
        printf("Skipping send/receive simulation — no photo stored earlier.\n");
    }

    // --- Complete ---
    print_section("FILE SYSTEM DEMO COMPLETE");
    printf("Data directories:\n  -> %s\n  -> %s\n", FS_BASE_PATH, FS_TRANSFER_PATH);

    return 0;
}
