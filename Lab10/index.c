#include <stdint.h>
#include <stdio.h>

#define SUCCESS 1

typedef struct {
    unsigned char filename[8];       // Short filename - e.g., "MYFILE  TXT"
    unsigned char extension[3];      // e.g., "TXT"
    unsigned char attributes;        // File attributes (read-only, hidden, directory, etc.)
    unsigned short creation_time;    // Time file was created
    unsigned short creation_date;    // Date file was created
    unsigned short last_access_date; // Last Access Date
    unsigned short last_write_time;
    unsigned short last_write_date;
    unsigned short first_cluster_high; // High word of first cluster number (for FAT32)
    unsigned short first_cluster_low;  // Low word of first cluster number
    unsigned long file_size;           // File size in bytes
} IndexEntry;

// How you might use it:
// To read a directory, you would read sectors from the disk that belong to that directory's data clusters.
// A directory is just a sequence of these 32-byte entries.

// Example: Reading the root directory (simplified for FAT16/FAT32 where root dir can be in clusters)
// Assume 'device' is your block device interface (e.g., SD card driver)
typedef struct BlockDevice {

} BlockDevice;

// Assume 'bpb' is your BIOS Parameter Block structure already read from boot sector
typedef struct BPB {
    unsigned int bytes_per_sector;
    unsigned int sectors_per_cluster;
} BIOSParameterBlock;

void list_directory_contents(BlockDevice *device, const BIOSParameterBlock *bpb, uint32_t directory_start_cluster) {
    uint8_t sector_buffer[bpb->bytes_per_sector];
    IndexEntry *dir_entry;
    uint32_t current_cluster = directory_start_cluster;

    // if (directory_start_cluster == 0 && bpb->fat_type == FAT32) { // FAT32 root dir
    //     current_cluster = bpb->root_cluster_fat32;
    // } else if (directory_start_cluster == 0 && (bpb->fat_type == FAT12 || bpb->fat_type == FAT16)) {
    //     // For FAT12/16, root directory is in a fixed location after FATs, not in clusters
    //     // Calculations for root_dir_sectors_start and num_root_dir_sectors are needed from BPB
    //     // This example will simplify and assume clustered directories for brevity
    //     printf("FAT12/16 root directory handling is different and not fully shown here.\n");
    //     return;
    // }

    while (current_cluster < 0x0FFFFFF8) {
        // While not end-of-chain or bad cluster
        
        uint32_t sector_of_cluster = get_sector_for_cluster(bpb, current_cluster); 
        // FIXME: 

        for (int i = 0; i < bpb->sectors_per_cluster; ++i) {
            if (device_read_sector(device, sector_of_cluster + i, sector_buffer) != SUCCESS) {
                printf("Error reading sector %lu\n", sector_of_cluster + i);
                return;
            }

            for (int j = 0; j < bpb->bytes_per_sector / sizeof(IndexEntry); ++j) {
                dir_entry = (IndexEntry *)&sector_buffer[j * sizeof(IndexEntry)];

                if (dir_entry->filename[0] == 0x00) { // No more entries in this part
                    return;                           // End of directory
                }
                if (dir_entry->filename[0] == 0xE5) { // Deleted entry
                    // printf("Deleted slot.\n");
                    continue;
                }

                // Print short file name
                char name[9];
                char ext[4];
                memcpy(name, dir_entry->filename, 8);
                memcpy(ext, dir_entry->extension, 3);
                name[8] = '\0';
                ext[3] = '\0';
                // Trim trailing spaces
                for (int k = 7; k >= 0 && name[k] == ' '; k--)
                    name[k] = '\0';
                for (int k = 2; k >= 0 && ext[k] == ' '; k--)
                    ext[k] = '\0';

                printf("File: %s.%s, Size: %lu, Start Cluster: %u\n",
                       name, ext,
                       dir_entry->file_size,
                       (uint32_t)dir_entry->first_cluster_high << 16 | dir_entry->first_cluster_low);
            }
        }
        current_cluster = read_fat_entry(device, bpb, current_cluster); 
        // FIXME:
        if (current_cluster == 0) {                                     
            // Should not happen if parsing correctly
            printf("Error: FAT chain broke unexpectedly.\n");
            break;
        }
    }
}