#!/bin/bash
# Q3. Create a shell program which simulates the behavior of top command.

get_proc_info() {
    echo "-------------------------------------------"
    ps -eo pid,%cpu,%mem,pri,comm --sort=+%cpu
    echo "-------------------------------------------"
}

get_info_by_pid() {
    echo "-------------------------------------------"
    read -p "Enter the PID: " pid

    if ! ps -p "$pid" > /dev/null 2>&1; then
        echo "Process with PID $pid does not exist."
        echo "-------------------------------------------"
        return
    fi

    echo ""
    echo -e "PID: $pid \n"
    echo -e "Command: $(ps -p $pid -o comm=) \n"
    echo -e "CPU Usage: $(ps -p $pid -o %cpu=) \n"
    echo -e "Memory Usage: $(ps -p $pid -o %mem=) \n"
    echo -e "Priority: $(ps -p $pid -o pri=) \n"
    echo -e "File Descriptors: $(lsof -p $pid 2>/dev/null | wc -l)"
    echo "-------------------------------------------"
}

top_live() {
    if command -v watch &> /dev/null; then
        watch -n 1 "ps aux --sort=-%cpu | head -20"
    else
        while true; do
            clear
            ps aux --sort=-%cpu | head -20
            sleep 1
        done
    fi
}

process_num() {
    num=$(ps -e | wc -l)
    echo "\nNumber of processes running = $num"
}

# Menu-based interface:
while true
do
    echo "-------------------------------------------"
    echo -e "Welcome to Nitin's Menu... \n"
    echo "1. Display current stats of all the running processes"
    echo "2. Display LIVE stats of all the running processes"
    echo "3. Show info by Process PID"
    echo "4. Show the number of currently running processes"
    echo "5. Exit."
    echo ""
    echo -e "Enter your choice: \c"
    read choice

    case $choice in 
        1)  
            get_proc_info
            ;;
        2)
            top_live
            ;;
        3)
            get_info_by_pid
            ;;
        4)
            process_num
            ;;
        5)
            exit 0;
            ;;
        *)
            echo "Please Enter a valid input"
            ;;
    esac
done