#!/bin/bash
# Q4. Write a script which takes a path as an input and make following changes in the directory.

change_files() {
    local dir="$1"

    if [! -d "$dir" ]
    then 
        echo "$dir is not a valid directory."
        return
    fi

    for p in "$dir"/*
    do  
        if [ -d "$p" ]
        then
            change_files "$p"
        elif [ -f "$p" ]
        then
            fname=$(basename "$p")
            dname=$(dirname "$p")
            ftype="${fname##*.}"
            name="${fname%.*}"

            uname=$(echo "$name" | tr 'a-z' 'A-Z')
            newname="$uname.$ftype"

            mv "$p" "$dname/$newname"

            if [[ "$newname" == *.txt ]]
            then
                temp="${newname%.txt}.tex"
                mv "$dname/$newname" "$dname/$temp"
            fi
        fi
    done
}

echo "Enter the path: "
read dp

if [ -d "$dp" ]
then    
    change_files "$dp"
else 
    echo "Invalid Option"
fi