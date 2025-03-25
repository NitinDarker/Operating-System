#!/bin/bash

# Scientific Calculator Script

# Define bc functions with extended precision
BC_LIB='
scale=10
define pi() { return 4*a(1) }
define to_rad(x) { return x*pi()/180 }
define mod(x,y) { return x%y }
define fact(n) { if (n<=1) return 1; return n*fact(n-1) }
'

show_menu() {
    clear
    echo "===== Scientific Calculator ====="
    echo "1. Add       2. Subtract"
    echo "3. Multiply  4. Divide"
    echo "5. Modulo    6. Power"
    echo "7. sin(x)    8. cos(x)"
    echo "9. tan(x)    10. ln(x)"
    echo "11. log10(x) 12. sqrt(x)"
    echo "13. e^x      14. |x|"
    echo "15. Factorial 16. Expression"
    echo "0. Exit"
    echo "================================"
}

get_number() {
    local prompt=$1
    local num
    
    while true; do
        read -p "$prompt" num
        if [[ $num =~ ^[+-]?([0-9]+(\.[0-9]*)?|\.[0-9]+)$ ]]; then
            echo "$num"
            return 0
        else
            echo "Error: Please enter a valid number (e.g. 5, -3.14, .75)"
        fi
    done
}

calculate() {
    local choice=$1
    local result
    
    case $choice in
    1|2|3|4|5|6) # Binary operations
        n1=$(get_number "Enter first number: ")
        n2=$(get_number "Enter second number: ")
        ;;
    *)
        n1=$(get_number "Enter number: ")  # Single argument operations
        ;;
    esac

    case $choice in
    1) result=$(echo "$BC_LIB; $n1 + $n2" | bc -l) ;;
    2) result=$(echo "$BC_LIB; $n1 - $n2" | bc -l) ;;
    3) result=$(echo "$BC_LIB; $n1 * $n2" | bc -l) ;;
    4) 
        if [[ $(echo "$n2 == 0" | bc -l) -eq 1 ]]; then
            echo "Error: Division by zero!" >&2
            return 1
        fi
        result=$(echo "$BC_LIB; $n1 / $n2" | bc -l)
        ;;
    5) result=$(echo "$BC_LIB; $n1 % $n2" | bc -l) ;;
    6) result=$(echo "$BC_LIB; $n1 ^ $n2" | bc -l) ;;
    7) result=$(echo "$BC_LIB; s(to_rad($n1))" | bc -l) ;;
    8) result=$(echo "$BC_LIB; c(to_rad($n1))" | bc -l) ;;
    9)
        cos_val=$(echo "$BC_LIB; c(to_rad($n1))" | bc -l)
        if [[ $(echo "$cos_val == 0" | bc -l) -eq 1 ]]; then
            echo "Error: Tangent undefined at $n1 degrees" >&2
            return 1
        fi
        result=$(echo "$BC_LIB; s(to_rad($n1))/$cos_val" | bc -l)
        ;;
    10)
        if [[ $(echo "$n1 <= 0" | bc -l) -eq 1 ]]; then
            echo "Error: ln requires positive number" >&2
            return 1
        fi
        result=$(echo "$BC_LIB; l($n1)" | bc -l)
        ;;
    11)
        if [[ $(echo "$n1 <= 0" | bc -l) -eq 1 ]]; then
            echo "Error: log10 requires positive number" >&2
            return 1
        fi
        result=$(echo "$BC_LIB; l($n1)/l(10)" | bc -l)
        ;;
    12)
        if [[ $(echo "$n1 < 0" | bc -l) -eq 1 ]]; then
            echo "Error: sqrt requires non-negative number" >&2
            return 1
        fi
        result=$(echo "$BC_LIB; sqrt($n1)" | bc -l)
        ;;
    13) result=$(echo "$BC_LIB; e($n1)" | bc -l) ;;
    14) result=$(echo "$BC_LIB; abs($n1)" | bc -l) ;;
    15)
        if [[ ! $n1 =~ ^[0-9]+$ || $n1 -lt 0 ]]; then
            echo "Error: Factorial requires non-negative integer" >&2
            return 1
        fi
        result=$(echo "$BC_LIB; fact($n1)" | bc -l)
        ;;
    16)
        read -p "Enter expression (numbers and +-*/^ only): " expr
        # Basic sanitization
        if [[ ! $expr =~ ^[0-9+\-*/\^\.\(\) ]+$ ]]; then
            echo "Error: Invalid characters in expression" >&2
            return 1
        fi
        result=$(echo "$BC_LIB; $expr" | bc -l 2>&1)
        if [[ $result == *"error"* ]]; then
            echo "Error: Invalid expression" >&2
            return 1
        fi
        ;;
    *)
        echo "Invalid option!" >&2
        return 1
        ;;
    esac

    printf "Result: %.10g\n" "$result"  # Clean up trailing zeros
}

# Main program
while true; do
    show_menu
    read -p "Enter choice (0-16): " choice
    case $choice in
        0) echo "Goodbye!"; exit 0 ;;
        [1-9]|1[0-6]) calculate "$choice" ;;
        *) echo "Invalid choice!" >&2 ;;
    esac
    read -p "Press Enter to continue..."
done