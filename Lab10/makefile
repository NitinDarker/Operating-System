CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 # -g for debugging
LDFLAGS =

# Source files
SRCS = wildlife_fs.c wildlife_fs_crypto.c main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = wildlife_tracker_fs_demo

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c wildlife_fs.h wildlife_fs_config.h wildlife_fs_types.h wildlife_fs_crypto.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf ./wildlife_data ./wildlife_transfer_area # Clean up created directories
