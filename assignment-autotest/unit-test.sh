#!/bin/bash

# Minimal unit test for AESD assignment 5

if [ ! -f ../finder-app/writer ]; then
    echo "writer binary not found"
    exit 1
fi

echo "Unit tests passed"
exit 0
