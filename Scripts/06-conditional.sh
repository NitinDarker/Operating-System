#!/bin/bash

### Conditional statements -> if/else
# Use square brackets '[]' for the if condition and separate characters with whitespaces

echo "Enter a number: "
read x
if [[ $x > 10 ]]; then
	echo "$x is greater than 1"
else
	echo "$x is smaller than 1"
fi