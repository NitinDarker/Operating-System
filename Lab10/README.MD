# Wildlife Tracker Basic File System

This project is a simple, C implementation of a basic file system designed for an embedded wildlife tracking device.

## Source Code

You can find the source code for this Lab project on GitHub:
[NitinDarker/Operating-System/Lab10](https://github.com/NitinDarker/Operating-System/tree/main/Lab10)

## What it Does:

*   **Stores Data:**
    *   Saves photos (simulated as binary data) with filenames based on timestamps and location tags.
    *   Records sensor readings (temperature, humidity, speed, light, time) into daily log files.
*   **Basic File Operations:** Provides functions to create, delete, open, close, write to, and read from files within its managed storage.
*   **Encryption:** Implements a simple XOR cipher for basic data privacy (for educational purposes, not production security).
*   **Organization:** Manages files within a dedicated base directory (`wildlife_data`) and uses a separate directory (`wildlife_transfer_area`) for simulated transfers.

## How to Compile and Run:

1.  **Prerequisites:**
    *   A C compiler (e.g., GCC)
    *   `make` utility

2.  **Compilation:**
    *   Open your terminal in the project's root directory (where the `Makefile` and source files are).
    *   Run the command:
        ```bash
        make
        ```
    *   This will compile the source code and create an executable named `wildlife_tracker_fs_demo`.

3.  **Running the Demonstration:**
    *   Execute the compiled program:
        ```bash
        ./wildlife_tracker_fs_demo
        ```
    *   The program will run through a series of tests, demonstrating photo storage/retrieval, sensor log storage/retrieval, file deletion, and simulated send/receive operations.
    *   It will print output to the console indicating its actions.
    *   Check the newly created directories `./wildlife_data` and `./wildlife_transfer_area` to see the files generated by the demo.

4.  **Cleaning Up:**
    *   To remove compiled object files, the executable, and the data directories:
        ```bash
        make clean
        ```

---