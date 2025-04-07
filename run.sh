#!/bin/bash

# Compile the server
echo "Compiling server..."
gcc server.c -o server

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Server compiled successfully!"
    
    # Run the server
    echo "Starting server..."
    ./server
else
    echo "Compilation failed!"
    exit 1
fi 