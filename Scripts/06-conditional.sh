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

## Arithmetic Comparisons:
# $a -gt $b ( $a > $b)
# $a -lt $b ( $a < $b)
# $a -ge $b ( $a >= $b)
# $a -le $b ( $a <= $b)
# $a -eq $b ( $a == $b)
# $a -ne $b ( $a != $b)


### String Comparisons:
echo "Enter your name:"
read name

echo "Enter your name: "
read name
if [ $name == "moto" ]; then
	echo "Hello moto"
else
	echo "You are not moto!"
fi

# [[ -z STR ]]        ->	Empty string
# [[ -n STR ]]        ->	Not empty string
# [[ STR == STR ]]    ->	Equal
# [[ STR = STR ]]     ->	Equal (Same above)
# [[ STR < STR ]]     ->	Less than (ASCII)
# [[ STR > STR ]]     ->	Greater than (ASCII)
# [[ STR != STR ]]    ->	Not Equal
# [[ STR =~ STR ]]    ->	Regexp


### File Comparisons:
# [[ -e FILE ]]	    ->  Exists
# [[ -d FILE ]]	    ->  Directory
# [[ -f FILE ]]	    ->  File
# [[ -h FILE ]]	    ->  Symlink
# [[ -s FILE ]]	    ->  Size is > 0 bytes
# [[ -r FILE ]]	    ->  Readable
# [[ -w FILE ]]	    ->  Writable
# [[ -x FILE ]]	    ->  Executable
# [[ f1 -nt f2 ]]	  ->  f1 newer than f2
# [[ f1 -ot f2 ]]	  ->  f2 older than f1
# [[ f1 -ef f2 ]]	  ->  Same files

file="test.txt"

if [ -f "$file" ]; then
    echo "$file exists"
else
    echo "$file does not exist"
fi

### Logical Operators
# [[ ! EXPR ]]	    ->  Not
# [[ X && Y ]]	    ->  And
# [[ X || Y ]]	    ->  Or

if [[ -f "$file" && -r "$file" ]]; then
    echo "$file is a readable file"
fi

# [[ X -a Y ]]  -> And
# [[ X -o Y ]]  -> Or


# if [ "$1" = 'y' -a $2 -gt 0 ]; then
#     echo "yes"
# fi
# if [ "$1" = 'n' -o $2 -lt 0 ]; then
#     echo "no"
# fi
