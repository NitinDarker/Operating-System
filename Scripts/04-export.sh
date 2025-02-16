#!/bin/bash

### Using a variable declared from terminal in the script
# When we run ./file.sh, a new interactive shell is created(fork+exec) for this file.
# So any variables from the previous source terminal are not present in this new terminal.
# This is where export command helps!

# Type the following commands on terminal before executing script:
# variable_name=value
# export variable_name
# ./04-export.sh
# Now this variable_name will be available in the shell script

echo "variable_name: $variable_name"
variable_name="hello"
echo "Variable changed in script"
echo "variable_name: $variable_name"

### How to run this script in parent shell?
# Using . before executing runs this script in our own shell (parent/source shell) rather than creating a new shell (no fork+exec)
# Type the following command in the terminal to run this script on the same shell:
# $ . ./04-export.sh
