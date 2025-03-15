#!/bin/bash

### Conditional statements -> if/else
# Use square brackets '[]' for the if condition and separate characters with whitespaces

# if statement
echo "If statement:"

if [[ 1 -eq 1 ]]; then
    echo "1 is equal to 1"
fi


echo "Enter a number:"
read num

if [[ $num -gt 10 ]]; then
    echo "The number is greater than 10"
fi


# if-else statement
echo "If-else statement:"

if [[ $num > 10 ]]; then
    echo "The number is greater than 10"
elif [[ $num -eq 10 ]]; then
    echo "The number is equal to 10"
else
    echo "The number is less than 10"
fi
