#include "filesystem.h"
#include <stdio.h>
#include <string.h>

// --- Example Usage ---

int main() {
    filesystem_init();

    // Create some files
    filesystem_create("readings1.dat", FILE_TYPE_READING);
    filesystem_create("photo1.jpg", FILE_TYPE_PHOTO);
    filesystem_create("readings2.dat", FILE_TYPE_READING);

    // Store data
    unsigned char data1[] = "Temperature: 25C, Humidity: 60%";
    filesystem_store("readings1.dat", data1, strlen((char *)data1));

    unsigned char data2[500]; // Simulate photo data
    for (int i = 0; i < 500; ++i)
        data2[i] = i % 256;
    filesystem_store("photo1.jpg", data2, 500);

    // Retrieve data
    unsigned char read_buffer[1024];
    int bytes_read = filesystem_retrieve("readings1.dat", read_buffer, sizeof(read_buffer));
    if (bytes_read > 0) {
        read_buffer[bytes_read] = '\0'; // Null terminate for printing as string
        printf("Retrieved data: %s\n", read_buffer);
    }

    bytes_read = filesystem_retrieve("photo1.jpg", read_buffer, sizeof(read_buffer));
    if (bytes_read > 0) {
        printf("Retrieved %d bytes from photo1.jpg\n", bytes_read);
        // You would typically process the binary data here
    }

    // Open and Close (basic functionality)
    int handle = filesystem_open("readings2.dat");
    if (handle != -1) {
        filesystem_close(handle);
    }

    // Delete a file
    filesystem_delete("readings1.dat");

    // Try to retrieve from deleted file
    bytes_read = filesystem_retrieve("readings1.dat", read_buffer, sizeof(read_buffer));
    if (bytes_read == -1) {
        printf("Correctly failed to retrieve from deleted file.\n");
    }

    // Close the file system storage files before exiting
    filesystem_close_storage();

    return 0;
}
