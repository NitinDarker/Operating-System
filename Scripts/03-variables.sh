#!/bin/bash

### Using variable
# Do not use white-spaces during assignment of variables
# '$' sign is used as a placeholder in echo

name="Nitin"
echo "Hey! I am $name"
# These shell variables can store any data type but they all are interpreted as strings by bash

# To read the input by the user, use read command
# It can even read whitspaces

echo "What is your name?"
read input
echo "Hello $input"

### Command substitution
# Use "$(command)" in echo to execute a command
# '$(command)' does not execute command, prints plain text

today=$(date +%A)
echo "Today is $today, $(date +%d-%m-%Y)"
