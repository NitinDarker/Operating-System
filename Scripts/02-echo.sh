#!/bin/bash

# Trying out the echo command in different scenerios
# echo commands prints on the terminal screen

echo Hello World
echo "Hello World"
echo "Hello  World"
echo Hello  World
echo "Hello \n World"
echo -e "Hello \n world" # Use -e flag to render escape sequence characters
echo Hello * World
echo "Hello * World"
echo "Hello" "  " "World"
echo Hello "  " World
echo Hello "World"
echo "Hello" World
echo "Hello "*" World" # Interesting, Why??
echo `hello world` # Backtick is used for commands, I guess
echo `hello` world
echo 'hello world' # Single quotes same as double quotes, Any differences?
echo 'hello' world
echo echo

# Also even when there is an error when I used backtick and command is not found by the bash, the script execution still continues, Good to know!

# '*' prints all the files in the current directory on a single line with space in-between.
echo *
