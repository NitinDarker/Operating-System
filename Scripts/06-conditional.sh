#!/bin/bash

### Conditional statements -> if/else
# Use square brackets '[]' for the if condition and separate characters with whitspaces

echo "Enter a number: "
read x
if [[ $x > 10 ]]; then
	echo "$x is greater than 1"
else
	echo "$x is smaller than 1"
fi

## Arithematic Comparisons:
# $a -gt $b ( $a > $b)
# $a -lt $b ( $a < $b)
# $a -ge $b ( $a >= $b)
# $a -le $b ( $a <= $b)
# $a -eq $b ( $a == $b)
# $a -ne $b ( $a != $b)

echo "Enter your name: "
read name
if [ $name == "Moto" ]; then
	echo "Hello Moto"
else
	echo "You are not moto!"
fi


