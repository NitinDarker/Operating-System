#!/bin/bash

### Using for loop

# Basic for loop
echo -n "Loop 1..."
for i in 1 2 3 4 5
do
	echo -n "$i "
done

echo -n "\nLoop 2..."
# For loop with sequence command (0 and 5 included)
for i in $(seq 0 5)
do 
	echo -n "$i"
done

# For loop with range (0 and 10 are included)
echo -n "\nLoop 3..."
for i in {0..10}
do
	echo -n $i " "
done

# For loop with range and step (jumps 2 steps)
echo -n "\nLoop 4..."
for i in {0..10..2}
do
	echo "$i "
done

# For loop using wildcard
for i in hello 1 * 2 goodbye
do
	echo "Loop 2... i = $i"
done

### Using while loop
