#!/bin/bash

### test statement -> true/false
# test statement are mostly used in if/else and loop condition block

# Numeric comparisons are restricted to integers only!
x=5; y=7; z=7.2
test $x -eq $y ; echo $?
test $x -lt $y ; echo $?
test $z -gt $y ; echo $?
test $z -eq $y ; echo $?

## Shorthand for test:
# Use square brackets '[]' for the if condition and separate every character with whitespaces
echo "Equal statements: \n"
test $x -eq $z
[ $x -eq $z ]

## Arithmetic Comparisons used by test:
# $a -gt $b ( $a > $b)
# $a -lt $b ( $a < $b)
# $a -ge $b ( $a >= $b)
# $a -le $b ( $a <= $b)
# $a -eq $b ( $a == $b)
# $a -ne $b ( $a != $b)


### String Comparisons used by test:
echo "Enter your name:"
read name

echo "Enter your name: "
read name
if [ $name == "moto" ]; then
	echo "Hello moto"
else
	echo "You are not moto!"
fi

# [[ -z STR ]]        ->	STR is an Empty string
# [[ -n STR ]]        ->	STR is Not an empty string
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
