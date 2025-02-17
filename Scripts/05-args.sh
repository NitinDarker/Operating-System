#!/bin/bash

### Passing arguments from the parent shell to the script
# Type the following on the terminal:
# $ ./04-args.sh Nitin 19 shell

echo "Shell name: $0"
echo "First Argument: $1"
echo "Second Argument: $2"
echo "Third Argument: $3"
echo "Fourth Argument: $4 (doesn't exist)"
echo "All arguments: $@"
echo "No. of arguments: $#"
echo "All arguments in a single string: $*"
echo "PID of shell process: $$"

# These command line arguments are stored in variable with numbers
# What if I try to reassign them?
# Ans- Cannot reassign because we cannot use numbers as variable name, I guess

num1=19
num2=$2
sum=$((num1+num2))
echo $sum
# Remember this syntax for addition of numbers
# Outer parenthesis is for command interpret like $(command)
# Inner parenthesis is for evaluating expression
