#!/bin/bash
# rename_files.sh
# This script recursively processes a given directory and:
#   a. Converts names of files (and directories) that are entirely in lowercase into uppercase.
#      If the uppercase name already exists, a version number is appended (e.g. ONE.01, ONE.02, etc.).
#   b. Changes files with the .txt extension into .tex (if the file name is in lowercase).
#   c. Applies the above changes recursively to any subdirectories.
#
# Usage: ./rename_files.sh <directory_path>

# Ensure exactly one argument is provided.
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <directory_path>"
    exit 1
fi

TARGET_DIR="$1"
if [ ! -d "$TARGET_DIR" ]; then``
    echo "Error: '$TARGET_DIR' is not a valid directory."
    exit 1
fi

# Function to process each directory recursively.
process_directory() {
    local dir="$1"
    
    # Loop through all items in the current directory.
    for item in "$dir"/*; do
        # If no item is found (e.g., empty directory), skip.
        [ -e "$item" ] || continue

        if [ -d "$item" ]; then
            # Recursively process the subdirectory first.
            process_directory "$item"
            
            # Rename the directory if its name is entirely lowercase.
            base=$(basename "$item")
            # Check if the name is all lowercase (allowing digits, dot, underscore, and hyphen).
            if [[ "$base" =~ ^[a-z0-9._-]+$ ]]; then
                new_base=$(echo "$base" | tr '[:lower:]' '[:upper:]')
                new_path="$(dirname "$item")/$new_base"
                # If a directory with the uppercase name already exists, add version suffix.
                if [ -e "$new_path" ]; then
                    version=1
                    while [ -e "$(dirname "$item")/${new_base}.$(printf "%02d" $version)" ]; do
                        version=$((version+1))
                    done
                    new_path="$(dirname "$item")/${new_base}.$(printf "%02d" $version)"
                fi
                mv "$item" "$new_path"
            fi

        elif [ -f "$item" ]; then
            base=$(basename "$item")
            # Process only if the file name is entirely in lowercase.
            if [ "$base" = "$(echo "$base" | tr '[:upper:]' '[:lower:]')" ]; then
                # Separate the base name and extension.
                if [[ "$base" == *.* ]]; then
                    name="${base%.*}"
                    ext=".${base##*.}"
                else
                    name="$base"
                    ext=""
                fi

                # Convert the name part to uppercase.
                new_name=$(echo "$name" | tr '[:lower:]' '[:upper:]')
                # If the extension is .txt, change it to .tex.
                if [ "$ext" = ".txt" ]; then
                    new_ext=".tex"
                else
                    new_ext=$(echo "$ext" | tr '[:lower:]' '[:upper:]')
                fi

                new_filename="${new_name}${new_ext}"
                new_path="$(dirname "$item")/$new_filename"
                # If a file with the new name already exists, append a version number.
                if [ -e "$new_path" ]; then
                    version=1
                    while [ -e "$(dirname "$item")/${new_name}.$(printf "%02d" $version)${new_ext}" ]; do
                        version=$((version+1))
                    done
                    new_path="$(dirname "$item")/${new_name}.$(printf "%02d" $version)${new_ext}"
                fi
                mv "$item" "$new_path"
            fi
        fi
    done
}

# Start processing from the target directory.
process_directory "$TARGET_DIR"

echo "Renaming complete."
