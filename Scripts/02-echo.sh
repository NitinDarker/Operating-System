#!/bin/bash

# Trying out the echo command in different scenarios
# echo commands prints its arguments on the terminal screen
# It separated all it arguments by a white-space

echo "Hello World"
echo Hello World
echo "Hello   World"
echo Hello   World
echo "Hello \n World"
echo -e "Hello \n world" # Use -e flag to render escape sequence characters
echo Hello * World  # '*' is a wildcard, it prints all the files in the current directory
echo "Hello * World"
echo "Hello" "  " "World"
echo Hello "  " World
echo Hello "World"
echo "Hello" World
echo "Hello "*" World" # Interesting, Why??
echo 'hello world' # Single quotes same as double quotes, Any differences?
echo 'hello' world
echo `hello world` # Backtick is used for commands, I guess
echo `hello` world
echo echo

# Also even when there is an error when I used backtick and command is not found by the bash, the script execution still continues, Good to know!

# '*' prints all the files in the current directory on a single line with space in-between.
echo *