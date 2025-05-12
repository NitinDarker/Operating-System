#ifndef WILDLIFE_FS_TYPES_H
#define WILDLIFE_FS_TYPES_H

#include <time.h>

typedef struct {
    time_t timestamp;
    float temperature_celsius;
    float humidity_percent;
    float speed_mps; // meters per second
    float light_lux;
    double latitude;
    double longitude;
} SensorReading;

// Enum for file types
typedef enum {
    FILE_TYPE_PHOTO,
    FILE_TYPE_SENSOR_LOG,
    FILE_TYPE_UNKNOWN
} FileType;

#endif