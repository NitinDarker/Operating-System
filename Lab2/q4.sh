#!/bin/bash
# Q4. Write a script which takes a path as an input and make following changes in the directory.

change_files() {
    local dir="$1" # Get the path as argument

    for p in "$dir"/*
    # Goes through every file/directory in the specified PATH
    do  
        if [ -d "$p" ]
        then
            change_files "$p"
            # If it is a directory then repeat the process
        elif [ -f "$p" ]
        then
            convert_to_uppercase "$p"
            convert_txt_to_tex "$p"
        fi
    done
}

convert_to_uppercase() {
    local file="$1"
    local dir=$(dirname -- "$file")
    local base=$(basename -- "$file")

    # `##`` is used to remove the longest matching prefix before the last . (dot).
    local ext="${base##*.}"

    # `%`` is used to remove the shortest matching suffix after the last . (dot).
    local name="${base%.*}"

    # there is ext of the file
    [[ $base == $ext ]] && ext=""

    # Convert name to uppercase
    local new_name="${name^^}"
    local count=1

    # Add version number if file already exists
    while [[ -e "$dir/$new_name.$ext" ]]; do
        formatted_count=$(printf "%02d" "$count")
        new_name="${name^^}.$formatted_count"
        ((count++))
    done

    mv "$dir/$name.$ext" "$dir/$new_name.$ext"
    echo "$dir/$new_name.$ext"
}

# *.txt --> .tex
convert_txt_to_tex() {
    local file="$1"
    local dir=$(dirname -- "$file")
    local base=$(basename -- "$file")

    if [[ "$base" == *.txt ]]; then
        temp="${base%.txt}.tex"
        mv "$dir/$base" "$dir/$temp"
    fi
}

# Execution starts from here:
echo "Enter the path: "
read dp

if [ -d "$dp" ]
then    
    change_files "$dp" # Call the change_files function
else 
    echo "Invalid Path" # If the given path is invalid
fi
