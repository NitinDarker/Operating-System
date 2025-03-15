#!/bin/bash
# Q1. Write down a shell script which lists top 10 processes using highest amount of main memory.

echo "Top 10 process with high memory usages"
echo "-------------------------------------------"
ps -eo pid,user,%mem,cmd --sort=-%mem | head -11
