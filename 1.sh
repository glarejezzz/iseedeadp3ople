#!/bin/bash

if [ -z "$1" ]; then
    ps aux
else
    ps -C "$1" -o pid,%cpu,%mem --no-headers | while read pid cpu mem; do
        echo "Process: $1"
        echo "PID: $pid"
        echo "CPU: $cpu%"
        echo "Memory: $mem%"
    done
fi
